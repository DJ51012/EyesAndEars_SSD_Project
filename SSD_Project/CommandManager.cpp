#include "CommandManager.h"


void CommandManager::printCommandGuide()
{
	cout << "[Usage] <cmd>" << endl;
	cout << "  <cmd>" << endl;
	cout << "    Read  - R <addr>" << endl;
	cout << "    Write - W <addr> <data>" << endl;
	cout << "    Erase - E <addr> <size>" << endl;
	cout << "    Flush - F" << endl;
	cout << "  <addr>" << endl;
	cout << "    0 - 99" << endl;
	cout << "  <data>" << endl;
	cout << "    8 digits [0-9][A-F]" << endl;
}

bool CommandManager::IsValidCommand(int argc, char* argv[])
{

	if (!IsArgumentExist(argc)) { 
		return false; 
	}
	if (!IsValidCommandCode(argv[1])) {
		return false; 
	}
	if (!IsValidCommandArguments(argc, argv)) {
		return false;
	}

	return true;
}

bool CommandManager::IsValidCommandArguments(int argc, char* argv[])
{
	switch (m_cmd)
	{
	case 'W':
		if (!IsValidWriteCommand(argc, argv)) { return false; }
		break;
	case 'R':
		if (!IsValidReadCommand(argc, argv)) { return false; }
		break;
	case 'E':
		if (!IsValidEraseCommand(argc, argv)) { return false; }
		break;
	case 'F':
		if (!IsValidFlushCommand(argc, argv)) { return false; }	
		break;
	default:
		break;
	}
	return true;
}

void CommandManager::executeSSDCommand(SsdDriver* ssd)
{
	switch (m_cmd)
	{
	case 'W': ssd->write(m_nLba, m_strData); break;
	case 'R': ssd->read(m_nLba); break;
	case 'E': ssd->erase(m_nLba, m_rangeSize); break;
	case 'F': ssd->flush(); break;
	default: break;
	}
	m_cmd = '-';
	return;
}

void CommandManager::doSsdCommand(SsdDriver* ssd, int argc, char* argv[])
{
	if (!IsValidCommand(argc, argv))
		return;
	executeSSDCommand(ssd);
	return;
}

bool CommandManager::IsArgumentExist(int argc)
{
	if (argc < 2) {
		throw::exception(ERROR_NO_COMMAND);
		return false;
	}
	return true;
}

bool CommandManager::IsValidCommandCode(char* cmd)
{
	if (strcmp(cmd, "W") != 0 && strcmp(cmd, "R") != 0 
		&& strcmp(cmd, "E") != 0 && strcmp(cmd, "F") != 0) {
		throw::exception(ERROR_COMMAND_CODE);
		return false;
	}
	m_cmd = cmd[0];
	return true;
}

bool CommandManager::IsValidReadCommand(int argc, char* argv[])
{
	if (!IsValidNumberOfArguments(argc, NR_READ_ARGC)) return false;
	if (!IsValidAddr(argv[2])) return false;

	return true;
}

bool CommandManager::IsValidWriteCommand(int argc, char* argv[])
{
	if (!IsValidNumberOfArguments(argc, NR_WRITE_ARGC)) return false;
	if (!IsValidAddr(argv[2])) return false;
	if (!IsValidData(argv[3])) return false;

	return true;
}

bool CommandManager::IsValidEraseCommand(int argc, char* argv[])
{
	if (!IsValidNumberOfArguments(argc, NR_ERASE_ARGC)) return false;
	if (!IsValidAddr(argv[2])) return false;
	if (!isValidRangeSize(argv[3])) return false;

	return true;
}

bool CommandManager::IsValidFlushCommand(int argc, char* argv[])
{
	if (!IsValidNumberOfArguments(argc, NR_FLUSH_ARGC)) return false;

	return true;
}

bool CommandManager::isValidRangeSize(string strRangeSize)
{
	auto size_str_ptr = strRangeSize.c_str();
	while (*size_str_ptr) {
		if (!std::isdigit(*size_str_ptr)) {
			throw::exception(ERROR_RANGE_SIZE);
		}
		++size_str_ptr;
	}

	auto value = stoi(strRangeSize);
	if (value > RANGE_SIZE_MAX) throw::exception(ERROR_RANGE_SIZE);

	m_rangeSize = value;
	return true;
}

bool CommandManager::IsValidNumberOfArguments(int argc, int nrCommandArguement)
{
	if (argc != nrCommandArguement) {
		throw::exception(ERROR_NR_ARGUMENTS);
		return false;
	}
	return true;
}

bool CommandManager::IsValidAddr(string strLba)
{
	for (auto ch : strLba)
		if (!IsNumber(ch)) {
			throw::exception(ERROR_ADDR);
			return false;
		}

	int nLba = stoi(strLba);
	if (!(nLba >= ADDR_MIN && nLba <= ADDR_MAX)) {
		throw::exception(ERROR_ADDR);
		return false;
	}

	m_nLba = nLba;
	return true;
}

bool CommandManager::IsValidData(string strData)
{
	// Format 0xXXXXXXXX
	if (strData.size() != DATA_WIDTH) {
		throw::exception(ERROR_DATA);
		return false;
	}
	if (strData[0] != '0' || strData[1] != 'x') {
		throw::exception(ERROR_DATA);
		return false;
	}

	for (int idx = 2; idx < DATA_WIDTH; idx++) {
		if (!(IsNumber(strData[idx]) || IsHexCapital(strData[idx]))) {
			throw::exception(ERROR_DATA);
			return false;
		}
	}

	m_strData = strData;
	return true;
}

bool CommandManager::IsNumber(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool CommandManager::IsHexCapital(char ch)
{
	return ch >= 'A' && ch <= 'F';
}

