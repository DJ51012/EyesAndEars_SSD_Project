#pragma once
#include <string>
#include <vector>
#include "../SSD_Project/SsdDriver.h"

using namespace std;

interface TestCmd {
	virtual void run_cmd(SsdDriver* ssd_driver, vector<string>& args) = 0;
};

class WriteTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, vector<string>& args) override {
		ssd_driver->write(stoi(args[0]), stoi(args[1]));
	}
};

class ExitTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, vector<string>& args) override {
		exit(0);
	}
};