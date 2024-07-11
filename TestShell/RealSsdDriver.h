#pragma once
#include "SsdDriver.h"
#include <cstdlib>
#include <stdexcept>
#include "../Logger/Logger.h"

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

	void erase(unsigned int lba_index, unsigned int size) override
	{
		string erase_cmd = string(SSD_EXECUTABLE) + " E " + to_string(lba_index) + " " + to_string(size);
		this->execute_cmd(erase_cmd, "Failed to invoke ssd erase command");
	}

	void flush() override
	{
		string flush_cmd = string(SSD_EXECUTABLE) + " F";
		this->execute_cmd(flush_cmd, "Failed to invoke ssd flush command");
	}


private:
	void execute_cmd(string& execute_cmd, string error_msg)
	{
		if (system(execute_cmd.c_str()) != 0) {
			throw runtime_error(error_msg);
		}
	}
};
