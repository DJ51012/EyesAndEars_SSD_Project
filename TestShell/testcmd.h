#pragma once
#include <string>
#include <vector>
#include <iostream>
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

class HelpTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, vector<string>& args) override {
		cout <<
			"write <idx> <value>    Write value to idx'th LBA.\n"\
			"read <idx>             Print out the value of idx'th LBA.\n";
			"exit                   Terminate the shell.\n";
			"help                   Print out the help message.\n";
			"fullwrite <value>      Write value to all LBAs indexed from 0 to 99.\n";
			"fullread <idx>         Print out all values of LBAs indexed from 0 to 99.\n";
	}
};

class FullwriteTestCmd : public TestCmd {
public:
	void run_cmd(SsdDriver* ssd_driver, vector<string>& args) override {
		for (auto idx = 0; idx < 100; idx++)
			ssd_driver->write(idx, stoi(args[0]));
	}
};