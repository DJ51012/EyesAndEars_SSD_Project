#include <cstdlib>
#include <stdexcept>
#include "RealSsdDriver.h"

#define SSD_EXECUTABLE "SSD.exe"

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

void RealSsdDriver::erase(unsigned int lba_index, unsigned int size)
{
	string erase_cmd = string(SSD_EXECUTABLE) + " E " + to_string(lba_index) + " " + to_string(size);
	this->execute_cmd(erase_cmd, "Failed to invoke ssd erase command");
}

void RealSsdDriver::execute_cmd(string& execute_cmd, string error_msg)
{
	if (system(execute_cmd.c_str()) != 0) {
		throw runtime_error(error_msg);
	}
}
