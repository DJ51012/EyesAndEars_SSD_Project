#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#define __TEST__
#include "../SSD_Project/Ssd.cpp"
#include "../SSD_Project/CommandManager.cpp"
#include "../SSD_Project/FileManager.cpp"
#include <string>
#include <iostream>

using namespace std;
using namespace testing;

// Command Manager

class SsdDriverMock : public SsdDriver
{
public:
	MOCK_METHOD(void, write, (unsigned int lba_index, string value), (override));
	MOCK_METHOD(void, read, (unsigned int lba_index), (override));
	MOCK_METHOD(void, erase, (unsigned int lba_index, unsigned int size), (override));
};

class CommandManagerFixture : public testing::Test
{
public:
	CommandManager cm;
	SsdDriverMock mock;
};

TEST_F(CommandManagerFixture, No_Command_Code)
{
	int argc = 1;
	char* argv[] = { "ssd" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("No Command"));
	}
}

TEST_F(CommandManagerFixture, No_Address_Read)
{
	int argc = 2;
	char* argv[] = { "ssd", "R" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid number of arguments for this command"));
	}
}

TEST_F(CommandManagerFixture, No_Address_Write)
{
	int argc = 2;
	char* argv[] = { "ssd", "W" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid number of arguments for this command"));
	}
}

TEST_F(CommandManagerFixture, No_Address_Erase)
{
	int argc = 2;
	char* argv[] = { "ssd", "E" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid number of arguments for this command"));
	}
}

TEST_F(CommandManagerFixture, Valid_Write)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "0", "0xABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = true;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Valid_Read)
{
	int argc = 3;
	char* argv[] = { "ssd", "R", "99" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = true;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_CommandCode000)
{
	int argc = 4;
	char* argv[] = { "ssd", "4", "0", "ABCD1234" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid command code"));
	}
}

TEST_F(CommandManagerFixture, Invalid_CommandCode001)
{
	int argc = 3;
	char* argv[] = { "ssd", "q", "0" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid command code"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Address000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "a", "ABCD1234" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid address"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Address001)
{
	int argc = 3;
	char* argv[] = { "ssd", "R", "100"};

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid address"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Address002)
{
	int argc = 4;
	char* argv[] = { "ssd", "E", "100", "10"};

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid address"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Data000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "50", "0xqw0-1231zlomas8=" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid data"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Data001)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "40", "0x!231+asd" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid data"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Range000)
{
	int argc = 4;
	char* argv[] = { "ssd", "E", "0", "11!@$%" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid range size"));
	}
}

TEST_F(CommandManagerFixture, Invalid_Range001)
{
	int argc = 4;
	char* argv[] = { "ssd", "E", "0", "11" };

	try {
		cm.IsValidCommand(argc, argv);
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), string("Invalid range size"));
	}
}

TEST_F(CommandManagerFixture, Execute_Write)
{
	// Arrange
	int argc = 4;
	char* argv[] = { "ssd", "W", "0", "0xABCD1234" };
	EXPECT_CALL(mock, write(0, "0xABCD1234"));

	//// Act
	if (cm.IsValidCommand(argc, argv))
		cm.executeSSDCommand(&mock);
}

TEST_F(CommandManagerFixture, Execute_Read)
{
	// Arrange
	int argc = 3;
	char* argv[] = { "ssd", "R", "99" };
	bool expected = true;
	EXPECT_CALL(mock, read(99));

	// Act
	if (cm.IsValidCommand(argc, argv))
		cm.executeSSDCommand(&mock);
}

TEST_F(CommandManagerFixture, Execute_Erase)
{
	// Arrange
	int argc = 4;
	char* argv[] = { "ssd", "E", "0", "10"};
	bool expected = true;
	EXPECT_CALL(mock, erase(0, 10));

	// Act
	if (cm.IsValidCommand(argc, argv))
		cm.executeSSDCommand(&mock);
}

TEST_F(CommandManagerFixture, Execute_Nothing)
{
	// Arrange
	EXPECT_CALL(mock, read)
		.Times(0);
	EXPECT_CALL(mock, write)
		.Times(0);

	// Act
	cm.executeSSDCommand(&mock);
}

//DriverFixture
class FileManagerMock : public FileManager {
public:
	MOCK_METHOD(void, writeNand, (unsigned int, string), ());
	MOCK_METHOD(void, readNand, (unsigned int), ());
};

class ssdDriverFixture : public testing::Test {
public:
	const int RESULT_READ_LINE = 0;
	const string DEFAULT_WRITE_VALUE = "0x00000000";
	FileManagerMock FMMock;
	Ssd ssd;
};

// invalid input 은 필터됐다고 가정
TEST_F(ssdDriverFixture, write_zero_and_check_nand_file_OK) {
	// Arrange
	unsigned int line = 10;
	string writeValue = DEFAULT_WRITE_VALUE;
	
	ssd.setFileManager(&FMMock);
	EXPECT_CALL(FMMock, writeNand(_,_)).Times(1);

	// Act
	ssd.write(line, writeValue);
}

TEST_F(ssdDriverFixture, write_non_zero_and_check_nand_file_OK) {
	// Arrange
	unsigned int line = 98;
	string writeValue = "0x12345678";

	ssd.setFileManager(&FMMock);
	EXPECT_CALL(FMMock, writeNand(_, _)).Times(1);
	
	// Act
	ssd.write(line, writeValue);
}

TEST_F(ssdDriverFixture, read_zero_and_check_result_file_OK) {
	// Arrange
	unsigned int line = 97;

	ssd.setFileManager(&FMMock);
	EXPECT_CALL(FMMock, readNand(_)).Times(1);
		
	// Act
	ssd.read(line);
}

TEST_F(ssdDriverFixture, read_non_zero_and_check_result_file_OK) {
	// Arrange
	unsigned int line = 98;
	string expected = "0x12345678";

	ssd.setFileManager(&FMMock);
	EXPECT_CALL(FMMock, readNand(_)).Times(1);
	
	// Act
	ssd.read(line);
}


// File Manager
class FileNotExistException : public exception {

};

class FileMangerFixture : public testing::Test {
protected:
	void SetUp() override {
		remove(NAND_FILE.c_str());	// delete "nand.txt"
	}
public:
	FileManager& fileManager = FileManager::getInstance();
	const string DATA1 = "0x00001004";
	const string DATA2 = "0x00C0FFEE";
	const string DEFAULT_WRITE_VALUE = "0x00000000";
	const string RESULT_FILE = "result.txt";
	const string NAND_FILE = "nand.txt";

	string getResultValue() {
		ifstream resultFile = getResultFile();
		string result = "";
		getline(resultFile, result);
		resultFile.close();
		return result;
	}

	string getWriteNandValue(int lba) {
		fstream resultFile = getNandFile();
		string result = "";
		for (int lbaIdx = 0; lbaIdx <= lba; lbaIdx++) {
			getline(resultFile, result);
		}
		resultFile.close();
		return result;
	}

	ifstream getResultFile() {
		ifstream resultFile(RESULT_FILE);
		if (resultFile.is_open() == false) {
			throw FileNotExistException();
		}
		return resultFile;
	}

	fstream getNandFile() {
		ifstream nandFileForCheck(NAND_FILE);
		if (nandFileForCheck.is_open() == false) {
			throw FileNotExistException();
		}
		nandFileForCheck.close();
		fstream nandfile(NAND_FILE);
		return nandfile;
	}
};

TEST_F(FileMangerFixture, ResultFileNotExist) {
	remove(RESULT_FILE.c_str());	// delete "result.txt"
	EXPECT_THROW(getResultValue(), FileNotExistException);
}

TEST_F(FileMangerFixture, NandFileNotExist) {
	EXPECT_THROW(getWriteNandValue(0), FileNotExistException);
}

TEST_F(FileMangerFixture, ReadTestInitLba0) {
	const int LBA = 0;
	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileMangerFixture, ReadTestInitLba99) {
	const int LBA = 99;
	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileMangerFixture, WriteTestInitLba1) {
	const int LBA = 1;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteTestInitLba98) {
	const int LBA = 98;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteReadTestLba0) {
	const int LBA = 0;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteReadTestLba99) {
	const int LBA = 99;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteReadTestOverWrite) {
	const int LBA = 10;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.writeNand(LBA, DATA2);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA2));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA2));
}

TEST_F(FileMangerFixture, WriteReadTestCleanLba) {
	const int WRITE_LBA = 15;
	const int READ_LBA = 20;
	fileManager.writeNand(WRITE_LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(WRITE_LBA), StrEq(DATA1));

	fileManager.readNand(READ_LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileMangerFixture, WriteReadTestConsecutiveWrite) {
	const int LBA1 = 40;
	const int LBA2 = 30;
	const int LBA3 = 99;

	fileManager.writeNand(LBA1, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA1), StrEq(DATA1));

	fileManager.writeNand(LBA2, DATA2);
	EXPECT_THAT(getWriteNandValue(LBA2), StrEq(DATA2));

	fileManager.writeNand(LBA3, DATA2);
	EXPECT_THAT(getWriteNandValue(LBA3), StrEq(DATA2));

	fileManager.readNand(LBA1);
	EXPECT_THAT(getResultValue(), StrEq(DATA1));

	fileManager.readNand(LBA2);
	EXPECT_THAT(getResultValue(), StrEq(DATA2));

	fileManager.readNand(LBA3);
	EXPECT_THAT(getResultValue(), StrEq(DATA2));
}

