#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class TestShell {
public:
	TestShell(string cmd, vector<string> args) : cmd(cmd), args(args) {

	}

	bool run_cmd() {
		AssertWrongCmd();
		AssertWrongArguments();

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
			
		throw invalid_argument("wrong argument for fullwrite!");
	}

	bool isValidLbaIndex(string& lba_index) {
		return stoi(lba_index) < 100;
	}

	bool isValidWriteValue(string& write_value) {
		return write_value.length() == 10 && write_value.substr(0, 2) == "0x";
	}

	string cmd;
	vector<string> args;
};