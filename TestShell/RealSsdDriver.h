#pragma once
#include "SsdDriver.h"
#include <cstdlib>
#include <stdexcept>

#define SSD_EXECUTABLE "ssd.exe"

class RealSsdDriver : public SsdDriver {
public:
	void write(unsigned int lba_index, string value) override
	{
		string write_cmd = string(SSD_EXECUTABLE) + " W " + to_string(lba_index) + " " + value;
		this->execute_cmd(write_cmd, "Failed to invoke ssd write command");
	}

	void read(unsigned int lba_index) override
	{
		string read_cmd = string(SSD_EXECUTABLE) + " R " + to_string(lba_index);
		this->execute_cmd(read_cmd, "Failed to invoke ssd read command");
	}

private:
	void execute_cmd(string& execute_cmd, string error_msg)
	{
		if (system(execute_cmd.c_str()) != 0) {
			throw runtime_error(error_msg);
		}
	}

};