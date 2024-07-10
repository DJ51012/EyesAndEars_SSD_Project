#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <stdio.h>
#include <stdexcept>

#include "MockSsdDriver.h"
#include "MockFileIo.h"
#include "../TestShell/SsdDriver.h"
#include "../TestShell/testshell.cpp"
#include "../TestShell/RealSsdDriver.cpp"
#include "../TestShell/RealFileIo.cpp"

using namespace std;
using namespace testing;

class TestShellFixture : public testing::Test {
public:
	bool test_cmd(const string& cmd, vector<string> args) {
		TestShell ts{ cmd, args, &mock_ssd, nullptr };

		return ts.run_cmd();
	}

	void set_expected_write_times(int times) {
		EXPECT_CALL(mock_ssd, write(_, _)).Times(times);
	}

	void set_expected_read_times(int times) {
		EXPECT_CALL(mock_ssd, read(_)).Times(times);
	}

	void backup_std_inout() {
		original_cin_buf = std::cin.rdbuf();
		original_cout_buf = std::cout.rdbuf();
	}

	void restore_std_inout() {
		std::cin.rdbuf(original_cin_buf);
		std::cout.rdbuf(original_cout_buf);
	}

	void set_std_inout(std::istringstream& in, std::ostringstream& out) {
		std::cin.rdbuf(in.rdbuf());
		std::cout.rdbuf(out.rdbuf());
	}

	MockSsdDriver mock_ssd;
	MockFileIO mfio;
	static const int READ_FAIL = 0x00;
	static const int READ_SUCCESS = 0x01;
	static const int ONE_LINE_SIZE = 10;
	static const int MAX_LBA_SIZE = 100;
private:
	std::streambuf* original_cin_buf;
	std::streambuf* original_cout_buf;
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

TEST_F(TestShellFixture, ReadCmdSuccess) {
	FILE* test_file = tmpfile();


	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1);

	mock_ssd.read(0x00);
	FILE* result_file = mfio.Open(FILE_NAME_RESULT, "w");
	ASSERT_EQ(result_file, test_file);
}

TEST_F(TestShellFixture, ReadCmdFail) {
	FILE* test_file = tmpfile();

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1);

	mock_ssd.read(0x00);
	FILE* result_file = mfio.Open("fail.txt", "r");
	ASSERT_NE(result_file, test_file);
}

TEST_F(TestShellFixture, ReadCmdTestShellSuccess) {
	FILE* test_file = tmpfile();
	const std::string fileNandContent = "0x000000000x000010040x00000000";
	std::string expect_result = "0x00001004";
	std::string result;
	EXPECT_CALL(mock_ssd, read(_))
		.Times(1)
		.WillRepeatedly(::testing::Invoke([&](unsigned int lba_index) {
			int position = lba_index * ONE_LINE_SIZE;
			result = fileNandContent.substr(position, 10);
		}));

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));

	EXPECT_CALL(mfio, Read(_, _, _))
		.WillOnce(::testing::Invoke([&](int fd, void* buf, size_t count) {
		memcpy(buf, result.c_str(), count);
		return count;
			}));

	//Test Start
	TestShell ts{ "read", { "1",}, &mock_ssd, &mfio };

	backup_std_inout();
	std::istringstream std_input;
	std::ostringstream std_output;
	set_std_inout(std_input, std_output);

	ts.run_cmd();

	EXPECT_THAT(std_output.str(), Eq(expect_result));

	restore_std_inout();
}

TEST_F(TestShellFixture, ReadCmdTestShellOpenReturnFail) {
	FILE* test_file = tmpfile();

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(nullptr));

	EXPECT_CALL(mock_ssd, read(_))
		.Times(1);

	TestShell ts{ "read", { "0",}, &mock_ssd, &mfio };
	EXPECT_THROW(ts.run_cmd(), std::runtime_error);
}

TEST_F(TestShellFixture, ExitCmd) {
	EXPECT_EXIT(test_cmd("exit", {}), ExitedWithCode(0), "");
}

TEST_F(TestShellFixture, HelpCmd) {
	backup_std_inout();

	std::istringstream std_input;
	std::ostringstream std_output;
	set_std_inout(std_input, std_output);

	EXPECT_TRUE(test_cmd("help", {}));

	string PRINT_OUT_HELP = 
		"write <idx> <value>    Write value to idx'th LBA.\n"\
		"read <idx>             Print out the value of idx'th LBA.\n"\
		"exit                   Terminate the shell.\n"\
		"help                   Print out the help message.\n"\
		"fullwrite <value>      Write value to all LBAs indexed from 0 to 99.\n"\
		"fullread <idx>         Print out all values of LBAs indexed from 0 to 99.\n";
	EXPECT_THAT(std_output.str(), Eq(PRINT_OUT_HELP));

	restore_std_inout();
}

TEST_F(TestShellFixture, FullWriteCmd) {
	set_expected_write_times(100);
	EXPECT_TRUE(test_cmd("fullwrite", { "0x12345678" }));
}

TEST_F(TestShellFixture, FullReadCmd) {
	FILE* test_file = tmpfile();
	std::string fileNandContent = "";
	for (int index=0; index < MAX_LBA_SIZE; index++) {
		std::stringstream ss;
		ss << std::hex << std::setw(8) << std::setfill('0') << index;
		std::string hexString = ss.str();
		std::string content = "0x" + hexString;
		fileNandContent.append(content);
	}
	std::string expected_str = fileNandContent;
	int expected_call = fileNandContent.size() / ONE_LINE_SIZE;
	std::string result;
	EXPECT_CALL(mock_ssd, read(_))
		.Times(expected_call)
		.WillRepeatedly(::testing::Invoke([&](unsigned int lba_index) {
			int position = lba_index * ONE_LINE_SIZE;
			result = fileNandContent.substr(position, 10);
		}));

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));


	EXPECT_CALL(mfio, Read(_, _, _))
		.WillRepeatedly(::testing::Invoke([&](int fd, void* buf, size_t count) {
			memset(buf, 0, count);
			memcpy(buf, result.c_str(), count);
			return count;
		}));

	TestShell ts{ TEST_CMD::FULLREAD, { }, &mock_ssd, &mfio };

	backup_std_inout();

	std::istringstream std_input;
	std::ostringstream std_output;
	set_std_inout(std_input, std_output);

	ts.run_cmd();

	EXPECT_THAT(std_output.str(), Eq(expected_str));

	restore_std_inout();
}

TEST_F(TestShellFixture, TestApp1Cmd) {
	FILE* test_file = tmpfile();
	std::string fileNandContent = "";
	for (int index = 0; index < MAX_LBA_SIZE; index++) {
		std::string content = "0x00000000";
		fileNandContent.append(content);
	}
	std::string expected_str = fileNandContent;
	int expected_call = fileNandContent.size() / ONE_LINE_SIZE;
	std::string result;
	EXPECT_CALL(mock_ssd, read(_))
		.Times(expected_call)
		.WillRepeatedly(::testing::Invoke([&](unsigned int lba_index) {
		int position = lba_index * ONE_LINE_SIZE;
		result = fileNandContent.substr(position, 10);
			}));

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));


	EXPECT_CALL(mfio, Read(_, _, _))
		.WillRepeatedly(::testing::Invoke([&](int fd, void* buf, size_t count) {
		memset(buf, 0, count);
		memcpy(buf, result.c_str(), count);
		return count;
			}));

	TestShell ts{ TEST_CMD::TESTAPP1, { }, &mock_ssd, &mfio };
	set_expected_write_times(100);

	ts.run_cmd();
}

TEST_F(TestShellFixture, TestApp2Cmd) {
	FILE* test_file = tmpfile();
	std::string fileNandContent = "";
	for (int index = 0; index < 6; index++) {
		std::string content = "0x12345678";
		fileNandContent.append(content);
	}
	std::string expected_str = fileNandContent;
	int expected_call = fileNandContent.size() / ONE_LINE_SIZE;
	std::string result;
	EXPECT_CALL(mock_ssd, read(_))
		.Times(expected_call)
		.WillRepeatedly(::testing::Invoke([&](unsigned int lba_index) {
		int position = lba_index * ONE_LINE_SIZE;
		result = fileNandContent.substr(position, 10);
			}));

	EXPECT_CALL(mfio, Open(_, _))
		.WillRepeatedly(Return(nullptr));
	EXPECT_CALL(mfio, Open(testing::StrEq(FILE_NAME_RESULT), _))
		.WillRepeatedly(Return(test_file));


	EXPECT_CALL(mfio, Read(_, _, _))
		.WillRepeatedly(::testing::Invoke([&](int fd, void* buf, size_t count) {
		memset(buf, 0, count);
		memcpy(buf, result.c_str(), count);
		return count;
			}));

	TestShell ts{ TEST_CMD::TESTAPP2, { }, &mock_ssd, &mfio };
	set_expected_write_times(186);

	ts.run_cmd();
}


TEST_F(TestShellFixture, SetUserInputString) {
	TestShell ts{ "", {}, &mock_ssd, nullptr };
	set_expected_write_times(1);

	ts.set_user_input("write 0 0x12345678");

	EXPECT_TRUE(ts.run_cmd());
}

TEST_F(TestShellFixture, InteractiveShell) {
	TestShell ts{ "", {}, &mock_ssd, nullptr };
	string test_user_inputs = 
		"write\n"
		"write 0 0x12345678\n"
		"exit\n";

	backup_std_inout();

	std::istringstream std_input(test_user_inputs);
	std::ostringstream std_output;
	set_std_inout(std_input, std_output);

	EXPECT_EXIT(ts.start_shell(), ExitedWithCode(0), "");

	restore_std_inout();
}

TEST(RealSsdDriver, ExceptionByExecutionFailure) {
	RealSsdDriver rsd;

	EXPECT_THROW(rsd.read(0), runtime_error);
	EXPECT_THROW(rsd.write(0, ""), runtime_error);
}

TEST(RealFileIo, ExceptionByExecutionFailure) {
	RealFileIo rfi;

	EXPECT_THROW(rfi.Open(FILE_NAME_NAND, "r"), runtime_error);
}