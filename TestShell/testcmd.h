#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "SsdDriver.h"
#include "FileIoInterface.h"
#include "util.h"

using namespace std;

interface TestCmd {
	virtual void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) = 0;
};

class WriteTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		ssd_driver->write(stoi(args[0]), args[1]);
	}
};

class ReadTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		ssd_driver->read(stoi(args[0]));
		FILE* fd = fio->Open(FILE_NAME_RESULT, "r");
		if (fd == nullptr) {
			throw std::runtime_error("File Open Error");
		}
		else {
			char buf[MAX_BUF_SIZE];
			memset(buf, 0, MAX_BUF_SIZE);
			fio->Read(_fileno(fd), buf, ONE_LINE_SIZE);
			buf[ONE_LINE_SIZE] = 0;
			fclose(fd);
			std::cout << buf;
		}
	}
	static const int ONE_LINE_SIZE = 10;
	static const int MAX_LINE = 100;
	static const int MAX_BUF_SIZE = ONE_LINE_SIZE * MAX_LINE;
};

class ExitTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		exit(0);
	}
};

class HelpTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		cout <<
			"write <idx> <value>    Write value to idx'th LBA.\n"\
			"read <idx>             Print out the value of idx'th LBA.\n"\
			"exit                   Terminate the shell.\n"\
			"help                   Print out the help message.\n"\
			"fullwrite <value>      Write value to all LBAs indexed from 0 to 99.\n"\
			"fullread <idx>         Print out all values of LBAs indexed from 0 to 99.\n";
	}
};

class FullwriteTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		for (auto idx = 0; idx < 100; idx++)
			ssd_driver->write(idx, args[0]);
	}
};

class FullreadTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		for(int index =0; index < MAX_LINE; index++){
			ssd_driver->read(index);
			FILE* fd = fio->Open(FILE_NAME_RESULT, "r");
			if (fd == nullptr) {
				throw std::runtime_error("File Open Error");
			}
			else {
				char buf[MAX_BUF_SIZE];
				memset(buf, 0, MAX_BUF_SIZE);
				int result = fio->Read(_fileno(fd), buf, ONE_LINE_SIZE);
				buf[ONE_LINE_SIZE] = 0;
				fclose(fd);
				if (result == 0) return;
				std::cout << buf;
			}
		}

	}
	static const int ONE_LINE_SIZE = 10;
	static const int MAX_LINE = 100;
	static const int MAX_BUF_SIZE = ONE_LINE_SIZE * MAX_LINE;
};

class TestApp1TestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		FullreadTestCmd full_read{};
		FullwriteTestCmd full_write{};

		args.clear();
		args.push_back("0x00000000");
		full_write.run_cmd(ssd_driver, fio, args);

		std::ostringstream test_out_stream;
		StdBufUtil std_buf_util;
		std_buf_util.change_stdout(&test_out_stream);

		args.clear();
		full_read.run_cmd(ssd_driver, fio, args);

		std_buf_util.restore_stdout();

		auto test_output = test_out_stream.str();
		for (size_t i = 0; i < 100; ++i) {
			if (test_output.substr(i * 10, 10) != "0x00000000") {
				cout << test_output << endl;
				throw std::runtime_error("TestApp1 failed.");
			}
		}
	}
};

class TestApp2TestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		ReadTestCmd read{};
		WriteTestCmd write{};

		for (int lba_index = 0; lba_index < 6; lba_index++) {
			args.clear();
			args.push_back(to_string(lba_index));
			args.push_back("0xAAAABBBB");
			for (int write_iter = 0; write_iter < 30; write_iter++) {
				write.run_cmd(ssd_driver, fio, args);
			}
		}
		
		for (int lba_index = 0; lba_index < 6; lba_index++) {
			args.clear();
			args.push_back(to_string(lba_index));
			args.push_back("0x12345678");
			write.run_cmd(ssd_driver, fio, args);
		}

		std::ostringstream test_out_stream;
		StdBufUtil std_buf_util;
		std_buf_util.change_stdout(&test_out_stream);

		for (int lba_index = 0; lba_index < 6; lba_index++) {
			args.clear();
			args.push_back(to_string(lba_index));
			read.run_cmd(ssd_driver, fio, args);
		}
		
		std_buf_util.restore_stdout();

		auto test_output = test_out_stream.str();
		for (size_t i = 0; i < 6; ++i) {
			if (test_output.substr(i * 10, 10) != "0x12345678") {
				cout << test_output << endl;
				throw std::runtime_error("TestApp2 failed.");
			}
		}
	}
};
