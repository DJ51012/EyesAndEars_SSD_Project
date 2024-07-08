#include <iostream>
#include <string>
#include "Ssd.h"

using namespace std;

class CommandManager
{
public:
	int getAddrData(int& addr, unsigned &data, int argc, char* argv[])
	{
		return 0;
	}


	void printCommandGuide()
	{
		cout << "[Usage] <cmd> <addr> <data>" << endl;
		cout << "  <cmd>" << endl;
		cout << "    Read  - R <addr>" << endl;
		cout << "    Write - W <addr> <data>" << endl;
		cout << "  <addr>" << endl;
		cout << "    0 - 99" << endl;
		cout << "  <data>" << endl;
		cout << "    8 digits [0-9][A-Z]" << endl;
	}

	bool IsValidCommand(int argc, char* argv[])
	{

		if (!IsArgumentExist(argc)) { return false; };
		if (!IsValidCommandCode(argv[1])) { return false; };

		return true;
	}

private:

	char cmd;
	int nLba;
	unsigned nData;

	bool IsArgumentExist(int argc)
	{
		return argc >= 3;
	}

	bool IsValidCommandCode(char* cmd)
	{
		if (strcmp(cmd, "W") == 0
			|| strcmp(cmd, "R") == 0)
			return true;
		return false;
	}
};
