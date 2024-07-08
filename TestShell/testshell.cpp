#include <string>
#include <vector>
#include <stdexcept>
#include "testcmd.h"
#include "../SSD_Project/SsdDriver.h"
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
	TestShell(string cmd, vector<string> args, SsdDriver* ssd_driver) 
		: cmd(cmd), args(args), ssd_driver(ssd_driver){

	}
	void setDriver(SsdDriver* driver)  {
		this->driver = driver;
	}
	void setFileIo(FileIoInterface* fio) {
		this->fio = fio;
	}

	bool run_cmd() {
		AssertWrongCmd();
		AssertWrongArguments();

		auto cmd_runner = get_test_cmd_runner();
		if (cmd_runner != nullptr) {
			cmd_runner->run_cmd(ssd_driver, args);
			return true;
		}

		return false;
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

		throw invalid_argument("Undefined test command!");
	}

	void AssertWrongArguments()
	{
		if (cmd == TEST_CMD::WRITE && args.size() >= 2 && isValidLbaIndex(args[0]) && isValidWriteValue(args[1])) return;
		if (cmd == TEST_CMD::READ && args.size() >= 1 && isValidLbaIndex(args[0])) return;
		if (cmd == TEST_CMD::FULLWRITE && args.size() >= 1 && isValidWriteValue(args[0])) return;
		if (cmd == TEST_CMD::EXIT) return;
		if (cmd == TEST_CMD::HELP) return;
			
		throw invalid_argument("Wrong argument!");
	}

	bool isValidLbaIndex(string& lba_index) {
		return stoi(lba_index) < 100;
	}

	bool isValidWriteValue(string& write_value) {
		return write_value.length() == 10 && write_value.substr(0, 2) == "0x";
	}

	TestCmd* get_test_cmd_runner() {
		if (cmd == TEST_CMD::WRITE) return new WriteTestCmd();
		if (cmd == TEST_CMD::EXIT) return new ExitTestCmd();
		if (cmd == TEST_CMD::HELP) return new HelpTestCmd();
		return nullptr;
	}

	string cmd;
	SsdDriver* driver;
	FileIoInterface* fio;
	vector<string> args;
	SsdDriver* ssd_driver = nullptr;
};