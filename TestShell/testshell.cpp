#include <string>
#include <vector>
#include <stdexcept>
#include "testcmd.h"
#include "../SSD_Project/SsdDriver.h"
#include "../TestShell/FileIOInterface.h"

using namespace std;

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
		if (cmd == "read") return;
		if (cmd == "write") return;
		if (cmd == "exit") return;
		if (cmd == "help") return;
		if (cmd == "fullread") return;
		if (cmd == "fullwrite") return;

		throw invalid_argument("Undefined test command!");
	}

	void AssertWrongArguments()
	{
		if (cmd == "write" && args.size() >= 2 && isValidLbaIndex(args[0]) && isValidWriteValue(args[1])) return;
		if (cmd == "read" && args.size() >= 1 && isValidLbaIndex(args[0])) return;
		if (cmd == "fullwrite" && args.size() >= 1 && isValidWriteValue(args[0])) return;
		if (cmd == "exit") return;
		if (cmd == "help") return;
			
		throw invalid_argument("Wrong argument!");
	}

	bool isValidLbaIndex(string& lba_index) {
		return stoi(lba_index) < 100;
	}

	bool isValidWriteValue(string& write_value) {
		return write_value.length() == 10 && write_value.substr(0, 2) == "0x";
	}

	TestCmd* get_test_cmd_runner() {
		if (cmd == "write") return new WriteTestCmd();
		if (cmd == "exit") return new ExitTestCmd();
		if (cmd == "help") return new HelpTestCmd();
		return nullptr;
	}

	string cmd;
	SsdDriver* driver;
	FileIoInterface* fio;
	vector<string> args;
	SsdDriver* ssd_driver = nullptr;
};