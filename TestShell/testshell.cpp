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
		if (cmd == "write" && args.size() >= 2 && stoi(args[0]) < 100 && args[1].length() == 10 && args[1].substr(0, 2) == "0x") return;
		if (cmd == "read" && args.size() >= 1 && stoi(args[0]) < 100) return;
		if (cmd == "fullwrite" && args.size() >= 1 && args[0].length() == 10 && args[0].substr(0, 2) == "0x") return;
			
		throw invalid_argument("wrong argument for fullwrite!");
	}

	string cmd;
	vector<string> args;
};