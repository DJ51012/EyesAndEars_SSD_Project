#include <cstdlib>
#include <stdexcept>
#include "RealSsdDriver.h"

#define SSD_EXECUTABLE "ssd.exe"

void RealSsdDriver::write(unsigned int lba_index, string value)
{
	string execute_cmd = string(SSD_EXECUTABLE) + " W " + to_string(lba_index) + " " + value;
	if (system(execute_cmd.c_str()) != 0) {
		throw runtime_error("Failed to invoke ssd write command");
	}
}

void RealSsdDriver::read(unsigned int lba_index)
{
	string execute_cmd = string(SSD_EXECUTABLE) + " R " + to_string(lba_index);
	if (system(execute_cmd.c_str()) != 0) {
		throw runtime_error("Failed to invoke ssd read command");
	}
}
