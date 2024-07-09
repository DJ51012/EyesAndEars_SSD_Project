#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "testcmd.h"
#include "SsdDriver.h"
#include "../TestShell/FileIOInterface.h"

using namespace std;

namespace TEST_CMD {
	const string WRITE = "write";
	const string READ = "read";
	const string EXIT = "exit";
	const string HELP = "help";
	const string FULLWRITE = "fullwrite";
	const string FULLREAD = "fullread";
}

class TestShell {
public:
	TestShell(string cmd, vector<string> args, SsdDriver* ssd_driver, FileIoInterface*  fio_interface)
		: cmd(cmd), args(args), ssd_driver(ssd_driver), fio(fio_interface){

	}
	void setDriver(SsdDriver* driver)  {
		this->ssd_driver = driver;
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

	void start_shell() {
		while (1) {
			cout << "TestShell> ";

			string user_input;

			getline(std::cin, user_input);
			this->set_user_input(user_input);

			try {
				this->run_cmd();
			}
			catch (std::invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	}

private:
	void AssertWrongCmd()
	{
		auto allowed_cmds = {
			TEST_CMD::WRITE, TEST_CMD::READ, TEST_CMD::EXIT, TEST_CMD::HELP,
			TEST_CMD::FULLWRITE, TEST_CMD::FULLREAD
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
		if (cmd == TEST_CMD::FULLREAD && args.size() == 0 ) return;
		if (cmd == TEST_CMD::EXIT) return;
		if (cmd == TEST_CMD::HELP) return;
			
		throw invalid_argument("WRONG ARGUMENT");
	}

	bool isValidLbaIndex(string& lba_index) {
		return stoi(lba_index) < 100;
	}

	bool isValidWriteValue(string& write_value) {
		return write_value.length() == 10 && write_value.substr(0, 2) == "0x";
	}

	TestCmd* get_test_cmd_runner() {
		if (cmd == TEST_CMD::WRITE) return new WriteTestCmd();
		if (cmd == TEST_CMD::READ) return new ReadTestCmd();
		if (cmd == TEST_CMD::EXIT) return new ExitTestCmd();
		if (cmd == TEST_CMD::HELP) return new HelpTestCmd();
		if (cmd == TEST_CMD::FULLWRITE) return new FullwriteTestCmd();
		if (cmd == TEST_CMD::FULLREAD) return new FullreadTestCmd();
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