#include <string>

using namespace std;

class TestShell {
public:
	TestShell(string cmd) : cmd(cmd) {

	}

	bool run_cmd() {
		return false;
	}

private:
	string cmd;
};