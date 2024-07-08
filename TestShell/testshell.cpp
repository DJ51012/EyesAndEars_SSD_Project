#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class TestShell {
public:
	TestShell(string cmd) : cmd(cmd) {

	}

	bool run_cmd() {
		AssertWrongCmd();
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

	string cmd;
};