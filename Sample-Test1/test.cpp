#include "pch.h"
#include "../SSD_Project/MySsdDriver.cpp"
#include <string>
#include <iostream>
#include <string>

using namespace std;
using namespace testing;

class ssdDriverFixture : public testing::Test {
public:
	MySsdDriver mySsdDriver;

	unsigned int readFileWithLine(ifstream &file, int line) {
        unsigned int result = 0;
		if (!file.is_open()) {
			std::cerr << "Error: Could not open file " << std::endl;
			return 0;
		}
		int line_idx = 0;
		while (file >> result) {
			line_idx++;
			if (line_idx == line) break;
		}

		std::string currentLine;
		while (std::getline(file, currentLine)) {
			line_idx++;
			if (line_idx == line) {
				try {
					return stoi(currentLine); // 현재 줄의 문자열을 정수로 변환하여 반환
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Error: The line does not contain a valid integer" << std::endl;
					return 0;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Error: The integer is out of range" << std::endl;
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
	unsigned int value = 0;

	mySsdDriver.write(line, value);
	ifstream nandFile("nand.txt");
	unsigned int actual = readFileWithLine(nandFile, line);
	unsigned int expected = value;

	EXPECT_EQ(expected, actual);
}

TEST_F(ssdDriverFixture, write_non_zero_and_check_nand_file_OK) {
	unsigned int line = 99;
	unsigned int value = 10;

	mySsdDriver.write(line, value);
	ifstream nandFile("nand.txt");
	unsigned int actual = readFileWithLine(nandFile, line);
	unsigned int expected = value;

	EXPECT_EQ(expected, actual);
}


TEST_F(ssdDriverFixture, read_zero_and_check_result_file_OK) {
	unsigned int line = 97;
	mySsdDriver.read(line);

	ifstream resultFile("result.txt");
	unsigned int actual = readFileWithLine(resultFile, line);
	unsigned int expected = 0;
	EXPECT_EQ(expected, actual);
}


TEST_F(ssdDriverFixture, read_non_zero_and_check_result_file_OK) {
	unsigned int line = 99;
	mySsdDriver.read(line);

	ifstream resultFile("result.txt");
	unsigned int actual = readFileWithLine(resultFile, line);
	unsigned int expected = 10;
	EXPECT_EQ(expected, actual);
}

