#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "SsdDriver.h"
#include "FileIoInterface.h"
#include "util.h"

using namespace std;

namespace TEST_CMD {
	const string WRITE = "write";
	const string READ = "read";
	const string EXIT = "exit";
	const string HELP = "help";
	const string FULLWRITE = "fullwrite";
	const string FULLREAD = "fullread";
}

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
			fio->Read((int)fd, buf, ONE_LINE_SIZE);
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

				int result = fio->Read((int)fd, buf, ONE_LINE_SIZE);
				if (result == 0) return;
				std::cout << buf;
				if (fd) fclose(fd);
			}
		}

	}
	static const int ONE_LINE_SIZE = 10;
	static const int MAX_LINE = 100;
	static const int MAX_BUF_SIZE = ONE_LINE_SIZE * MAX_LINE;
};
