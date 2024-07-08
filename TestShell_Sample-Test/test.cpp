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
	// TODO: Test for exit
}

TEST(TestShell, HelpCmd) {
	// TODO: Test for help
}

TEST(TestShell, FullWriteCmd) {
	// TODO: Test for fullwrite
}

TEST(TestShell, FullReadCmd) {
	// TODO: Test for fullread
}
