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
};

class TestShellFixture : public testing::Test {
public:
	bool test_cmd(const string& cmd, vector<string> args) {
		TestShell ts{ cmd, args, &mock_ssd, nullptr };

		return ts.run_cmd();
	}

	void set_expected_write_times(int times) {
		EXPECT_CALL(mock_ssd, write(_, _)).Times(times);
	}

	MockSsdDriver mock_ssd;
};

TEST_F(TestShellFixture, WrongCmd) {
	EXPECT_THROW(test_cmd("undefined_cmd", {}), invalid_argument);
}

TEST_F(TestShellFixture, WrongUserArguments) {
	EXPECT_THROW(test_cmd("write", {}), invalid_argument);
	EXPECT_THROW(test_cmd("write", { "" }), invalid_argument);
	EXPECT_THROW(test_cmd("write", { "100", "0x12345678" }), invalid_argument);
	EXPECT_THROW(test_cmd("read", {}), invalid_argument);
	EXPECT_THROW(test_cmd("read", { "100" }), invalid_argument);
	EXPECT_THROW(test_cmd("fullwrite", {}), invalid_argument);
	EXPECT_THROW(test_cmd("fullwrite", { "0x1234" }), invalid_argument);
	EXPECT_THROW(test_cmd("fullwrite", { "1234567890" }), invalid_argument);
}

TEST_F(TestShellFixture, WriteCmd) {
	set_expected_write_times(1);
	EXPECT_TRUE(test_cmd("write", { "0", "0x12345678" }));
}

TEST_F(SSDFixture, ReadCmdSuccess) {
	FILE* test_file = tmpfile();


	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(Return(READ_SUCCESS));

	mock_ssd.read(0x00);
	FILE* result_file = mfio.Open(FILE_NAME_RESULT, "w");
	ASSERT_EQ(result_file, test_file);
}

TEST_F(SSDFixture, ReadCmdFail) {
	FILE* test_file = tmpfile();

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(Return(READ_SUCCESS));

	mock_ssd.read(0x00);
	FILE* result_file = mfio.Open("fail.txt", "r");
	ASSERT_NE(result_file, test_file);
}

TEST_F(SSDFixture, ReadCmdTestShellSuccess) {
	FILE* test_file = tmpfile();

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(Return(READ_SUCCESS));

	TestShell ts{ "read", { "0",}, &mock_ssd, &mfio };

	ts.run_cmd();
}

TEST_F(SSDFixture, ReadCmdTestShellFail) {
	FILE* test_file = tmpfile();

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(nullptr));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(Return(READ_SUCCESS));

	TestShell ts{ "read", { "0",}, &mock_ssd, &mfio };
	EXPECT_THROW(ts.run_cmd(), std::runtime_error);
}

TEST_F(TestShellFixture, ExitCmd) {
	EXPECT_EXIT(test_cmd("exit", {}), ExitedWithCode(0), "");
}

TEST_F(TestShellFixture, HelpCmd) {
	stringstream captured_buf;
	streambuf* backup_cout_buf = std::cout.rdbuf();
	std::cout.rdbuf(captured_buf.rdbuf());

	EXPECT_TRUE(test_cmd("help", {}));

	string PRINT_OUT_HELP = 
		"write <idx> <value>    Write value to idx'th LBA.\n"\
		"read <idx>             Print out the value of idx'th LBA.\n"\
		"exit                   Terminate the shell.\n"\
		"help                   Print out the help message.\n"\
		"fullwrite <value>      Write value to all LBAs indexed from 0 to 99.\n"\
		"fullread <idx>         Print out all values of LBAs indexed from 0 to 99.\n";
	EXPECT_THAT(captured_buf.str(), Eq(PRINT_OUT_HELP));

	std::cout.rdbuf(backup_cout_buf);
}

TEST_F(TestShellFixture, FullWriteCmd) {
	set_expected_write_times(100);
	EXPECT_TRUE(test_cmd("fullwrite", { "0x12345678" }));
}

TEST(TestShell, FullReadCmd) {
	// TODO: Test for fullread
}
