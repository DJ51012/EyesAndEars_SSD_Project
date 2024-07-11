#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
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


// File Manager
class FileNotExistException : public exception {

};

class FileManagerFixture : public testing::Test {
protected:
	void SetUp() override {
		remove(NAND_FILE.c_str());	// delete "nand.txt"
		remove(COMMAND_BUFFER.c_str());	// delete "buffer.txt"
	}
public:
	Ssd ssd;

	FileManager& fileManager = FileManager::getInstance();
	const string DATA1 = "0x00001004";
	const string DATA2 = "0x00C0FFEE";
	const string DEFAULT_WRITE_VALUE = "0x00000000";
	const string RESULT_FILE = "result.txt";
	const string NAND_FILE = "nand.txt";
	const string COMMAND_BUFFER = "buffer.txt";

	string getResultValue() {
		ifstream resultFile = getResultFile();
		string result = "";
		getline(resultFile, result);
		resultFile.close();
		return result;
	}

	string getWriteNandValue(int lba) {
		ifstream resultFile = getNandFile();
		string result = "";
		for (int lbaIdx = 0; lbaIdx <= lba; lbaIdx++) {
			getline(resultFile, result);
		}
		resultFile.close();
		return result;
	}

	bool isInCommandBuffer(string cmd) {
		ifstream commandBuffer = getCommandBuffer();
		string readCmd = "";
		while (getline(commandBuffer, readCmd)) {
			if (cmd == readCmd) {
				commandBuffer.close();
				return true;
			}
		}
		commandBuffer.close();
		return false;
	}

	ifstream getResultFile() {
		ifstream resultFile(RESULT_FILE);
		if (resultFile.is_open() == false) {
			throw FileNotExistException();
		}
		return resultFile;
	}

	ifstream getNandFile() {
		ifstream nandFileForCheck(NAND_FILE);
		if (nandFileForCheck.is_open() == false) {
			throw FileNotExistException();
		}
		nandFileForCheck.close();
		ifstream nandFile(NAND_FILE);
		return nandFile;
	}

	ifstream getCommandBuffer() {
		ifstream cmdBufferForCheck(COMMAND_BUFFER);
		if (cmdBufferForCheck.is_open() == false) {
			throw FileNotExistException();
		}
		ifstream commandBuffer(COMMAND_BUFFER);
		return commandBuffer;
	}
};

TEST_F(FileManagerFixture, ResultFileNotExist) {
	remove(RESULT_FILE.c_str());	// delete "result.txt"
	EXPECT_THROW(getResultValue(), FileNotExistException);
}

TEST_F(FileManagerFixture, NandFileNotExist) {
	EXPECT_THROW(getWriteNandValue(0), FileNotExistException);
}

TEST_F(FileManagerFixture, ReadTestInitLba0) {
	const int LBA = 0;
	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileManagerFixture, ReadTestInitLba99) {
	const int LBA = 99;
	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileManagerFixture, WriteTestInitLba1) {
	const int LBA = 1;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));
}

TEST_F(FileManagerFixture, WriteTestInitLba98) {
	const int LBA = 98;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));
}

TEST_F(FileManagerFixture, WriteReadTestLba0) {
	const int LBA = 0;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA1));
}

TEST_F(FileManagerFixture, WriteReadTestLba99) {
	const int LBA = 99;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA1));
}

TEST_F(FileManagerFixture, WriteReadTestOverWrite) {
	const int LBA = 10;
	fileManager.writeNand(LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA1));

	fileManager.writeNand(LBA, DATA2);
	EXPECT_THAT(getWriteNandValue(LBA), StrEq(DATA2));

	fileManager.readNand(LBA);
	EXPECT_THAT(getResultValue(), StrEq(DATA2));
}

TEST_F(FileManagerFixture, WriteReadTestCleanLba) {
	const int WRITE_LBA = 15;
	const int READ_LBA = 20;
	fileManager.writeNand(WRITE_LBA, DATA1);
	EXPECT_THAT(getWriteNandValue(WRITE_LBA), StrEq(DATA1));

	fileManager.readNand(READ_LBA);
	EXPECT_THAT(getResultValue(), StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileManagerFixture, WriteReadTestConsecutiveWrite) {
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

TEST_F(FileManagerFixture, CommandBufferNotExist) {
	const string cmd = "W 20 0x1234ABCD";
	EXPECT_THROW(isInCommandBuffer(cmd), FileNotExistException);
}

TEST_F(FileManagerFixture, WriteCommandBufferFile) {
	const string cmd = "W 20 0x1234ABCD";

	fileManager.writeBuffer(cmd);

	bool expected = true;
	bool actual = isInCommandBuffer(cmd);

	EXPECT_EQ(expected, actual);
}

TEST_F(FileManagerFixture, WriteCommandBufferFileMoreThanOneCommand) {
	const string cmd1 = "W 20 0x1234ABCD";
	const string cmd2 = "W 21 0x1234ABCD";

	fileManager.writeBuffer(cmd1);
	fileManager.writeBuffer(cmd2);

	bool expected = true;
	bool actual1 = isInCommandBuffer(cmd1);
	bool actual2 = isInCommandBuffer(cmd2);

	EXPECT_EQ(expected, actual1);
	EXPECT_EQ(expected, actual2);
}

TEST_F(FileManagerFixture, WriteCommandBufferFileMoreThanTenCommands) {
	const string cmd1 = "W 10 0x1234AB01";
	const string cmd2 = "W 10 0x1234AB02";

	for(int i = 0 ; i<10 ; i++)
		fileManager.writeBuffer(cmd1);
	fileManager.writeBuffer(cmd2);

	bool expected1 = false;
	bool actual1 = isInCommandBuffer(cmd1);
	bool expected2 = true;
	bool actual2 = isInCommandBuffer(cmd2);

	EXPECT_EQ(expected1, actual1);
	EXPECT_EQ(expected2, actual2);
}


TEST_F(FileManagerFixture, WriteAndReadCommandBufferFile) {
	const string cmd = "W 20 0x1234ABCD";
	fileManager.writeBuffer(cmd);
	EXPECT_THAT(fileManager.readBuffer(), Contains(cmd));
}

TEST_F(FileManagerFixture, writeToCommandBuffer) {
	// Arrange
	unsigned int line = 98;
	string writeValue = "0x12345678";
	const string cmd = "W " + to_string(line) + " " + writeValue;
	// Act
	ssd.write(line, writeValue);
	bool expected = true;
	bool actual = isInCommandBuffer(cmd);
	
	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(FileManagerFixture, ReadFromCommandBuffer) {
	// Arrange
	unsigned int line = 98;
	string writeValue = "0x12345678";
	const string cmd = "W " + to_string(line) + " " + writeValue;

	// Act
	fileManager.writeBuffer(cmd);
	ssd.read(line);

	string expected = writeValue;
	string actual = getResultValue();

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(FileManagerFixture, ReadFromNandFile) {
	// Arrange
	unsigned int line = 98;
	string writeValue = "0x12345678";

	// Act
	fileManager.writeNand(line, writeValue);
	ssd.read(line);

	string expected = writeValue;
	string actual = getResultValue();

	// Assert
	EXPECT_EQ(expected, actual);
}
