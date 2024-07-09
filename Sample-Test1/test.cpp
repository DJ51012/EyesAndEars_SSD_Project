#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../SSD_Project/Ssd.cpp"
#include "../SSD_Project/CommandManager.h"
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
