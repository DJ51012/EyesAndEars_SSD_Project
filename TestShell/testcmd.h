#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "../SSD_Project/SsdDriver.h"
#include "FileIoInterface.h"

using namespace std;

interface TestCmd {
	virtual void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) = 0;
};

class WriteTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		ssd_driver->write(stoi(args[0]), stoi(args[1]));
	}
};

class ReadTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, FileIoInterface* fio, vector<string>& args) override {
		ssd_driver->read(stoi(args[0]));
		if (fio->Open(FILE_NAME_RESULT, "r") == nullptr) {
			throw std::runtime_error("File Open Error");
		}
	}
};