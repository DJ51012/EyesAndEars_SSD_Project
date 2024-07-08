#include <iostream>
#include <string>
#include "Ssd.h"

using namespace std;

class CommandManager
{
public:
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

		switch (m_cmd)
		{
		case 'W':
			if (!IsValidWriteCommand(argc, argv)) { return false; }
			break;
		case 'R':
			if (!IsValidReadCommand(argc, argv)) { return false; }
			break;
		default:
			break;
		}

		return true;
	}


private:

	char m_cmd;
	int m_nLba;
	string m_strData;

	bool IsArgumentExist(int argc)
	{
		return argc >= 2;
	}

	bool IsValidCommandCode(char* cmd)
	{
		if (strcmp(cmd, "W") == 0 || strcmp(cmd, "R") == 0) {
			m_cmd = cmd[0];
			return true;
		}
		return false;
	}

	bool IsValidReadCommand(int argc, char* argv[])
	{
		if (argc != 3) return false;
		if (!IsValidAddr(argv[2])) return false;

		return true;
	}

	bool IsValidWriteCommand(int argc, char* argv[])
	{
		if (argc != 4) return false;
		if (!IsValidAddr(argv[2])) return false;
		if (!IsValidData(argv[3])) return false;

		return true;
	}

	bool IsValidAddr(string strLba)
	{
		for (auto ch : strLba)
			if (ch < '0' || ch > '9')
				return false;

		int nLba = stoi(strLba);
		if (!(nLba >= 0 && nLba <= 99))
			return false;

		m_nLba = nLba;
		return true;
	}

	bool IsValidData(string strData)
	{
		// Format 0xXXXXXXXX
		if (strData.size() != 10) return false;
		if (strData[0] != '0' || strData[1] != 'x') return false;

		for (int idx = 2; idx < 10; idx++) {
			if (!((strData[idx] >= '0' && strData[idx] <= '9')
				|| (strData[idx] >= 'A' && strData[idx] <= 'Z')))
				return false;
		}

		m_strData = strData;
		return true;
	}
};
