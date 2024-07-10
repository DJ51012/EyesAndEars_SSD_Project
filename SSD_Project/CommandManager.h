#include <iostream>
#include <string>
#include "SsdDriver.h"

using namespace std;

class CommandManager
{
public:

	void doSsdCommand(SsdDriver* ssd, int argc, char* argv[]);
	bool IsValidCommand(int argc, char* argv[]);
	void executeSSDCommand(SsdDriver* ssd);

	char const * const ERROR_NO_COMMAND = "No Command";
	char const * const ERROR_COMMAND_CODE = "Invalid command code";
	char const * const ERROR_NR_ARGUMENTS= "Invalid number of arguments for this command";
	char const * const ERROR_ADDR= "Invalid address";
	char const * const ERROR_DATA= "Invalid data";
	char const * const ERROR_RANGE_SIZE= "Invalid range size";
private:

	char m_cmd = '-';
	int m_nLba;
	string m_strData;
	int m_rangeSize;

	const int DATA_WIDTH = 10;
	const int ADDR_MAX = 99;
	const int ADDR_MIN = 0;
	const int NR_WRITE_ARGC = 4;
	const int NR_READ_ARGC = 3;
	const int NR_ERASE_ARGC = 4;
	const int RANGE_SIZE_MAX = 10;

	void printCommandGuide();

	bool IsValidCommandArguments(int argc, char* argv[]);
	bool IsArgumentExist(int argc);
	bool IsValidCommandCode(char* cmd);
	bool IsValidReadCommand(int argc, char* argv[]);
	bool IsValidWriteCommand(int argc, char* argv[]);
	bool IsValidEraseCommand(int argc, char* argv[]);
	bool isValidRangeSize(string strRangeSize);
	bool IsValidNumberOfArguments(int argc, int nrCommandArguement);
	bool IsValidAddr(string strLba);
	bool IsValidData(string strData);
	bool IsNumber(char ch);
	bool IsHexCapital(char ch);
};
