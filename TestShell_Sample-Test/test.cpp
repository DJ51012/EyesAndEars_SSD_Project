#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <stdio.h>
#include <stdexcept>

#include "../SSD_Project/SsdDriver.h"

#include "MockSsdDriver.h"
#include "MockFileIo.h"
#include "../TestShell/testshell.cpp"

using namespace std;
using namespace testing;

class SSDFixture : public testing::Test {
protected:
	void SetUp() override {
	}
public:
	MockSsdDriver mock_ssd;
	MockFileIO mfio;
	static const int READ_FAIL = 0x00;
	static const int READ_SUCCESS = 0x01;
	const char* FILE_NAME_RESULT = "result.txt";
	const char* FILE_NAME_BAD_RESULT = "result_wrong_name.txt";
};

TEST(TestShell, WrongCmd) {
	TestShell ts{ "undefined_cmd", {}, nullptr };

	EXPECT_THROW(ts.run_cmd(), invalid_argument);
}

TEST(TestShell, WrongUserArguments) {
	TestShell ts_write_1{ "write" , {}, nullptr };
	TestShell ts_write_2{ "write" , { "" }, nullptr };
	TestShell ts_write_3{ "write" , { "100", "0x12345678"}, nullptr };
	TestShell ts_read_1{ "read" , {}, nullptr };
	TestShell ts_read_2{ "read" , { "100" }, nullptr };
	TestShell ts_fullwrite_1{ "fullwrite" , {}, nullptr };
	TestShell ts_fullwrite_2{ "fullwrite" , { "0x1234" }, nullptr };
	TestShell ts_fullwrite_3{ "fullwrite" , { "1234567890" }, nullptr };

	EXPECT_THROW(ts_write_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_write_2.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_write_3.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_read_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_read_2.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_fullwrite_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_fullwrite_2.run_cmd(), invalid_argument);
}

TEST(TestShell, WriteCmd) {
	MockSsdDriver msd;

	EXPECT_CALL(msd, write(_, _)).Times(1);

	TestShell ts{ "write", { "0", "0x12345678" }, &msd };

	ts.run_cmd();
}

TEST_F(SSDFixture, BasicReadCmdSuccess) {
	FILE test_file;
	EXPECT_CALL(mfio, Open(FILE_NAME_RESULT, _))
		.WillRepeatedly(Return(&test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(Return(READ_SUCCESS));

	mock_ssd.read(0x00);
	mfio.Open(FILE_NAME_RESULT, "w");
}

TEST(TestShell, ExitCmd) {
	TestShell ts_exit{ "exit" , {}, nullptr };

	EXPECT_EXIT(ts_exit.run_cmd(), ExitedWithCode(0), "");
}

TEST(TestShell, HelpCmd) {
	stringstream captured_buf;
	streambuf* backup_cout_buf = std::cout.rdbuf();
	std::cout.rdbuf(captured_buf.rdbuf());

	TestShell ts_help{ "help" , {}, nullptr };
	EXPECT_TRUE(ts_help.run_cmd());

	string PRINT_OUT_HELP = 
		"write <idx> <value>    Write value to idx'th LBA.\n"\
		"read <idx>             Print out the value of idx'th LBA.\n";
		"exit                   Terminate the shell.\n";
		"help                   Print out the help message.\n";
		"fullwrite <value>      Write value to all LBAs indexed from 0 to 99.\n";
		"fullread <idx>         Print out all values of LBAs indexed from 0 to 99.\n";
	EXPECT_THAT(captured_buf.str(), Eq(PRINT_OUT_HELP));

	std::cout.rdbuf(backup_cout_buf);
}

TEST(TestShell, FullWriteCmd) {
	// TODO: Test for fullwrite
}

TEST(TestShell, FullReadCmd) {
	// TODO: Test for fullread
}
