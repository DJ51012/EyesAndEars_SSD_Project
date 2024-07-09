#include "pch.h"
#include "../SSD_Project/MySsdDriver.cpp"
#include "../SSD_Project/CommandManager.h"
#include <string>
#include <iostream>

using namespace std;
using namespace testing;

// Command Manager
class CommandManagerFixture : public testing::Test
{
public:
	CommandManager cm;
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
	char* argv[] = { "ssd", "W", "0", "ABCD1234" };

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

//DriverFixture
class ssdDriverFixture : public testing::Test {
public:
	MySsdDriver mySsdDriver;

	string readFileWithLine(ifstream& file, int line) {
		string result;
		if (!file.is_open()) {
			std::cerr << "Error: Could not open file " << std::endl;
			return 0;
		}
		int line_idx = 0;
		while (file >> result) {
			line_idx++;
			if (line_idx == line) break;
		}

		string currentLine;
		while (getline(file, currentLine)) {
			line_idx++;
			if (line_idx == line) {
				try {
					return currentLine; // 현재 줄의 문자열을 정수로 변환하여 반환
				}
				catch (const invalid_argument& e) {
					cerr << "Error: The line does not contain a valid integer" << std::endl;
					return 0;
				}
				catch (const out_of_range& e) {
					cerr << "Error: The integer is out of range" << std::endl;
					return 0;
				}
			}
		}

		return result;
	}
};

// invalid input 은 필터됐다고 가정
TEST_F(ssdDriverFixture, write_zero_and_check_nand_file_OK) {
	unsigned int line = 98;
	string value ="0";

	mySsdDriver.write(line, value);
	ifstream nandFile("nand.txt");
	string actual = readFileWithLine(nandFile, line);
	string expected = value;

	EXPECT_EQ(expected, actual);
}

TEST_F(ssdDriverFixture, write_non_zero_and_check_nand_file_OK) {
	unsigned int line = 99;
	string value = "10";

	mySsdDriver.write(line, value);
	ifstream nandFile("nand.txt");
	string actual = readFileWithLine(nandFile, line);
	string expected = value;

	EXPECT_EQ(expected, actual);
}


TEST_F(ssdDriverFixture, read_zero_and_check_result_file_OK) {
	unsigned int line = 97;
	mySsdDriver.read(line);

	ifstream resultFile("result.txt");
	string actual = readFileWithLine(resultFile, line);
	string expected = "0";
	EXPECT_EQ(expected, actual);
}


TEST_F(ssdDriverFixture, read_non_zero_and_check_result_file_OK) {
	unsigned int line = 99;
	mySsdDriver.read(line);

	ifstream resultFile("result.txt");
	string actual = readFileWithLine(resultFile, line);
	string expected = "10";
	EXPECT_EQ(expected, actual);
}






