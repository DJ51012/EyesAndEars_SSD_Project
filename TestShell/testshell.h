#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "testcmd.h"
#include "SsdDriver.h"
#include "../TestShell/FileIOInterface.h"
#include "../ScenarioLibrary/ScenarioLibrary.h"
#include "CommandSet.h"
#include "ScenarioRunner.h"

using namespace std;

struct ParsedCommand
{
	string m_cmd;
	vector<string> m_args;
};

class TestShell {
public:
	TestShell(string cmd, vector<string> args, SsdDriver* ssd_driver, FileIoInterface* fio_interface);
	void setDriver(SsdDriver* driver);
	void setFileIo(FileIoInterface* fio);
	bool run_cmd();
	void set_user_input(const string& user_input);
	ParsedCommand parseUserInput(string user_input);
	void start_shell();

private:
	void AssertWrongCmd();
	void AssertWrongArguments();
	bool isValidLbaIndex(string& lba_index);
	bool isValidWriteValue(string& write_value);
	TestCmd* get_test_cmd_runner();
	void clearUserData();

	string cmd;
	FileIoInterface* fio;
	vector<string> args;
	SsdDriver* ssd_Driver = nullptr;
};