#include <cstdlib>
#include <stdexcept>
#include "RealSsdDriver.h"

#define SSD_EXECUTABLE "ssd.exe"

void RealSsdDriver::write(unsigned int lba_index, string value)
{
	string write_cmd = string(SSD_EXECUTABLE) + " W " + to_string(lba_index) + " " + value;
	this->execute_cmd(write_cmd, "Failed to invoke ssd write command");
}

void RealSsdDriver::read(unsigned int lba_index)
{
	string read_cmd = string(SSD_EXECUTABLE) + " R " + to_string(lba_index);
	this->execute_cmd(read_cmd, "Failed to invoke ssd read command");
}

void RealSsdDriver::execute_cmd(string& execute_cmd, string error_msg)
{
	if (isRedirection) {
		execute_cmd.append(" 2>nul");
	}
	if (system(execute_cmd.c_str()) != 0) {
		throw runtime_error(error_msg);
	}
}

void RealSsdDriver::setRedirection(bool redirection) {
	isRedirection = redirection;
}
