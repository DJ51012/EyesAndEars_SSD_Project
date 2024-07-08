#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/testshell.cpp"

TEST(TestShell, WrongCmd) {
	TestShell ts{ "undefined_cmd" , {} };

	EXPECT_THROW(ts.run_cmd(), invalid_argument);
}

TEST(TestShell, WrongUserArguments) {
	TestShell ts_write_1{ "write" , {} };
	TestShell ts_write_2{ "write" , { "" }};
	TestShell ts_write_3{ "write" , { "100", "0x12345678"}};
	TestShell ts_read_1{ "read" , {}};
	TestShell ts_read_2{ "read" , { "100" }};
	TestShell ts_fullwrite_1{ "fullwrite" , {}};
	TestShell ts_fullwrite_2{ "fullwrite" , { "0x1234" }};
	TestShell ts_fullwrite_3{ "fullwrite" , { "1234567890" }};

	EXPECT_THROW(ts_write_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_write_2.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_write_3.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_read_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_read_2.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_fullwrite_1.run_cmd(), invalid_argument);
	EXPECT_THROW(ts_fullwrite_2.run_cmd(), invalid_argument);
}

TEST(TestShell, WriteCmd) {
	// TODO: Test for write
}

TEST(TestShell, ReadCmd) {
	// TODO: Test for read
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
