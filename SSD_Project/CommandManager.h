#include <iostream>
#include <string>
#include "SsdDriver.h"

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

	void executeSSDCommand(SsdDriver* ssd)
	{
		switch (m_cmd)
		{
		case 'W': ssd->write(m_nLba, m_strData); break;
		case 'R': ssd->read(m_nLba); break;
		default: break;
		}
		m_cmd = '-';
		return;
	}

private:

	char m_cmd = '-';
	int m_nLba;
	string m_strData;

	const int DATA_WIDTH = 10;
	const int ADDR_MAX = 99;
	const int ADDR_MIN = 0;

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
			if (!IsNumber(ch))
				return false;

		int nLba = stoi(strLba);
		if (!(nLba >= ADDR_MIN && nLba <= ADDR_MAX))
			return false;

		m_nLba = nLba;
		return true;
	}

	bool IsValidData(string strData)
	{
		// Format 0xXXXXXXXX
		if (strData.size() != DATA_WIDTH) return false;
		if (strData[0] != '0' || strData[1] != 'x') return false;

		for (int idx = 2; idx < DATA_WIDTH; idx++) {
			if (!(IsNumber(strData[idx]) || IsHexCapital(strData[idx])))
				return false;
		}

		m_strData = strData;
		return true;
	}

	bool IsNumber(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	bool IsHexCapital(char ch)
	{
		return ch >= 'A' && ch <= 'F';
	}
};
