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
	TestShell ts{ "undefined_cmd" };

	EXPECT_FALSE(ts.run_cmd());
}

TEST(TestShell, WrongUserArguments) {
	// TODO: Check invalid user arguments
}

TEST(TestShell, WriteCmd) {
	// TODO: Test for write
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
