#include <string>

using namespace std;

class TestShell {
public:
	TestShell(string cmd) : cmd(cmd) {

	}

	int run_cmd() {
		return 0;
	}

private:
	string cmd;
};