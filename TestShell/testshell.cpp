#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include "testcmd.h"
#include "SsdDriver.h"
#include "util.h"
#include "../TestShell/FileIOInterface.h"
#include "../Logger/Logger.h"
#include "../ScenarioLibrary/ScenarioLibrary.h"
#include "CommandSet.h"
#include "ScenarioRunner.h"

using namespace std;

class TestShell {
public:
	TestShell(string cmd, vector<string> args, SsdDriver* ssd_driver, FileIoInterface* fio_interface)
		: cmd(cmd), args(args), ssd_driver(ssd_driver), fio(fio_interface) {

	}
	void setDriver(SsdDriver* driver) {
		this->ssd_driver = driver;
	}

	SsdDriver* getDriver() {
		return ssd_driver;
	}

	void setFileIo(FileIoInterface* fio) {
		this->fio = fio;
	}

	bool run_cmd() {
		AssertWrongCmd();
		AssertWrongArguments();

		auto cmd_runner = get_test_cmd_runner();
		if (cmd_runner != nullptr) {
			cmd_runner->run_cmd(ssd_driver, fio, args);
			delete cmd_runner;
			return true;
		}

		return false;
	}

	void set_user_input(const string& user_input) {
		istringstream user_input_stream{ user_input };
		string arg;
		bool isFirstArg = true;

		clearUserData();

		while (getline(user_input_stream, arg, ' ')) {
			if (arg.empty()) continue;

			if (isFirstArg) {
				this->cmd = arg;
				isFirstArg = false;
			}
			else {
				this->args.push_back(arg);
			}
		}
	}

	int run_shell(string cmd) {
		this->set_user_input(cmd);

		ScenarioCaller* sc = getScenarioCaller();

		if (sc->isValidScenario(this->cmd, this->args))
		{
			ScenarioRunner* sr = new ScenarioRunner;
			bool ret = sr->runScenario(sc->callScenario(this->cmd, this->args));
			if (ret)
				cout << "SCENARIO PASS" << endl;
			else
				cout << "SCENARIO FAIL" << endl;
			return ret;
		}

		try {
			this->run_cmd();
		}
		catch (std::invalid_argument& e) {
			cout << e.what() << endl;
			return -1;
		}
		catch (std::runtime_error& e) {
			cout << e.what() << endl;
			return -1;
		}
		return 0;
	}

	void start_shell() {
		while (1) {
			cout << "TestShell> ";
			string user_input;
			getline(std::cin, user_input);

			run_shell(user_input);
		}
	}

private:
	void AssertWrongCmd()
	{
		auto allowed_cmds = {
			TEST_CMD::WRITE, TEST_CMD::READ, TEST_CMD::EXIT, TEST_CMD::HELP,
			TEST_CMD::FULLWRITE, TEST_CMD::FULLREAD,
			TEST_CMD::ERASE, TEST_CMD::ERASERANGE, TEST_CMD::FLUSH
		};
		for (auto& cmd : allowed_cmds) {
			if (this->cmd == cmd) return;
		}

		throw invalid_argument("INVALID COMMAND");
	}

	void AssertWrongArguments()
	{
		if (cmd == TEST_CMD::WRITE && args.size() >= 2 && isValidLbaIndex(args[0]) && isValidWriteValue(args[1])) return;
		if (cmd == TEST_CMD::READ && args.size() >= 1 && isValidLbaIndex(args[0])) return;
		if (cmd == TEST_CMD::FULLWRITE && args.size() >= 1 && isValidWriteValue(args[0])) return;
		if (cmd == TEST_CMD::FULLREAD && args.size() == 0) return;
		if (cmd == TEST_CMD::EXIT) return;
		if (cmd == TEST_CMD::HELP) return;
		if (cmd == TEST_CMD::ERASE && args.size() >= 2 && isValidLbaIndex(args[0])) {
			if (std::all_of(args[1].begin(), args[1].end(), [](unsigned char c) {
				return std::isdigit(c);
				})) return;
		}
		if (cmd == TEST_CMD::ERASERANGE && args.size() >= 2 && isValidEraseRangeArgs()) return;
		if (cmd == TEST_CMD::FLUSH && args.size() == 0) return;


		throw invalid_argument("WRONG ARGUMENT");
	}

	bool isValidLbaIndex(string& lba_index) {
		try {
			auto lba_num = stoi(lba_index);
			return (lba_num < CONSTANTS::LBA_INDEX_MAX && lba_num >= CONSTANTS::LBA_INDEX_MIN);
		}
		catch (...) {}

		return false;
	}

	bool isValidLbaEndRangeMax(string& lba_index) {
		try {
			auto lba_num = stoi(lba_index);
			return (lba_num <= CONSTANTS::LBA_INDEX_MAX && lba_num >= CONSTANTS::LBA_INDEX_MIN);
		}
		catch (...) {}

		return false;
	}

	bool isValidEraseRangeArgs() {
		if (!isValidLbaIndex(args[0])) return false;
		if (!isValidLbaEndRangeMax(args[1])) return false;
		if (stoi(args[0]) > stoi(args[1])) return false;
		return true;
	}

	bool isValidWriteValue(string& write_value) {
		return (write_value.length() == CONSTANTS::VALID_VALUE_SIZE &&
			write_value.substr(0, 2) == CONSTANTS::VALID_VALUE_PREFIX);
	}

	TestCmd* get_test_cmd_runner() {
		if (cmd == TEST_CMD::WRITE) return new WriteTestCmd();
		if (cmd == TEST_CMD::READ) return new ReadTestCmd();
		if (cmd == TEST_CMD::EXIT) return new ExitTestCmd();
		if (cmd == TEST_CMD::HELP) return new HelpTestCmd();
		if (cmd == TEST_CMD::FULLWRITE) return new FullwriteTestCmd();
		if (cmd == TEST_CMD::FULLREAD) return new FullreadTestCmd();
		if (cmd == TEST_CMD::ERASE) return new EraseTestCmd();
		if (cmd == TEST_CMD::ERASERANGE) return new EraseRangeTestCmd();
		if (cmd == TEST_CMD::FLUSH) return new FlushTestCmd();
		return nullptr;
	}

	void clearUserData()
	{
		this->cmd = "";
		this->args.clear();
	}

	string cmd;
	FileIoInterface* fio;
	vector<string> args;
	SsdDriver* ssd_driver = nullptr;
};