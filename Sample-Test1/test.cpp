#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../SSD_Project/Ssd.cpp"
#include "../SSD_Project/CommandManager.h"
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
	MOCK_METHOD(string, read, (unsigned int lba_index), (override));
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

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Read)
{
	int argc = 1;
	char* argv[] = { "ssd", "R" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Write)
{
	int argc = 1;
	char* argv[] = { "ssd", "W" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
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

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_CommandCode001)
{
	int argc = 3;
	char* argv[] = { "ssd", "q", "0" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Address000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "a", "ABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Address001)
{
	int argc = 4;
	char* argv[] = { "ssd", "R", "100", "ABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}
TEST_F(CommandManagerFixture, Invalid_Data000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "50", "0xqw0-1231zlomas8=" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Data001)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "40", "0x!231+asd" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Execute_Write)
{
	// Arrange
	int argc = 4;
	char* argv[] = { "ssd", "W", "0", "0xABCD1234" };
	bool expected = true;
	EXPECT_CALL(mock, write(0, "0xABCD1234"));

	// Act
	bool actual = cm.IsValidCommand(argc, argv);
	cm.executeSSDCommand(&mock);

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Execute_Read)
{
	// Arrange
	int argc = 3;
	char* argv[] = { "ssd", "R", "99" };
	bool expected = true;
	EXPECT_CALL(mock, read(99));

	// Act
	bool actual = cm.IsValidCommand(argc, argv);
	cm.executeSSDCommand(&mock);

	// Assert
	EXPECT_EQ(expected, actual);
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
class ssdDriverFixture : public testing::Test {
public:
	Ssd Ssd;
	const int RESULT_READ_LINE = 0;

	string readFileWithLine(ifstream& file, int line) {
		string result;
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << endl;
			return "";
		}

		int line_idx = 0;
		string currentLine;
		while (getline(file, currentLine)) {
			if (line_idx == line) {
				return currentLine; // 현재 줄의 문자열을 정수로 변환하여 반환
			}
			line_idx++;
		}
		return result;
	}

	ifstream resultFile;
	ifstream nandFile;
};

// invalid input 은 필터됐다고 가정
TEST_F(ssdDriverFixture, write_zero_and_check_nand_file_OK) {
	// Arrange
	unsigned int line = 0;
	string writeValue = Ssd.DEFAULT_WRITE_VALUE;

	// Act
	Ssd.write(line, writeValue);
	nandFile.open(Ssd.NAND_FILE_NAME);
	string actual = readFileWithLine(nandFile, line);
	string expected = writeValue;

	// Assert
	EXPECT_EQ(expected, actual);
}


TEST_F(ssdDriverFixture, write_non_zero_and_check_nand_file_OK) {
	// Arrange
	unsigned int line = 98;
	string writeValue = "0x12345678";

	// Act
	Ssd.write(line, writeValue);
	nandFile.open(Ssd.NAND_FILE_NAME);
	string actual = readFileWithLine(nandFile, line);
	string expected = writeValue;

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(ssdDriverFixture, read_zero_and_check_result_file_OK) {
	// Arrange
	unsigned int line = 97;
	string expected = Ssd.DEFAULT_WRITE_VALUE;

	// Act
	Ssd.read(line);
	resultFile.open(Ssd.RESULT_FILE_NAME);
	string actual = readFileWithLine(resultFile, RESULT_READ_LINE);

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(ssdDriverFixture, read_non_zero_and_check_result_file_OK) {
	// Arrange
	unsigned int line = 98;
	string expected = "0x12345678";

	// Act
	Ssd.read(line);
	resultFile.open(Ssd.RESULT_FILE_NAME);
	string actual = readFileWithLine(resultFile, RESULT_READ_LINE);
	
	// Assert
	EXPECT_EQ(expected, actual);
}


// File Manager

class FileMangerFixture : public testing::Test {
public:
	FileManager& fileManager = FileManager::getInstance();
	const string DATA1 = "0x00001004";
	const string DATA2 = "0x00C0FFEE";
	const string DEFAULT_WRITE_VALUE = "0x00000000";
	
	ifstream getResultFile() {
		ifstream resultFile("result.txt");
		if (resultFile.is_open() == false) {
			// FAIL
		}
		return resultFile;
	}

	string getResultValue(ifstream& resultFile, int lba) {
		string result = "";
		getline(resultFile, result);
		return result;
	}
};

TEST_F(FileMangerFixture, ReadTestInitLba0) {
	fileManager.readNand(0);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 0);
	EXPECT_THAT(result, StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileMangerFixture, ReadTestInitLba99) {
	fileManager.readNand(99);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 99);
	EXPECT_THAT(result, StrEq(DEFAULT_WRITE_VALUE));
}

TEST_F(FileMangerFixture, WriteReadTestLba0) {
	fileManager.writeNand(0, DATA1);
	fileManager.readNand(0);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 0);
	EXPECT_THAT(result, StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteReadTestLba99) {
	fileManager.writeNand(99, DATA1);
	fileManager.readNand(99);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 99);
	EXPECT_THAT(result, StrEq(DATA1));
}

TEST_F(FileMangerFixture, WriteReadTestOverWrite) {
	fileManager.writeNand(10, DATA1);
	fileManager.writeNand(10, DATA2);
	fileManager.readNand(10);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 10);
	EXPECT_THAT(result, StrEq(DATA2));
}

TEST_F(FileMangerFixture, WriteReadTestCleanLba) {
	fileManager.writeNand(15, DATA1);
	fileManager.readNand(20);
	ifstream resultFile = getResultFile();
	string result = getResultValue(resultFile, 20);
	EXPECT_THAT(result, StrEq(DEFAULT_WRITE_VALUE));
}
