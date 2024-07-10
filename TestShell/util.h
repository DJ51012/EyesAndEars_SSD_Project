#pragma once
#include<iostream>
#include<sstream>

using namespace std;

namespace CONSTANTS {
	const int LBA_INDEX_MAX = 100;
	const int LBA_INDEX_MIN = 0;
	const int VALID_VALUE_SIZE = 10;
	const string VALID_VALUE_PREFIX = "0x";
}

class StdBufUtil {
public:
	void change_stdout(ostringstream* target_cout_buf) {
		this->original_cout_buf = std::cout.rdbuf();
		std::cout.rdbuf(target_cout_buf->rdbuf());
	}
	
	void restore_stdout() {
		std::cout.rdbuf(original_cout_buf);
		original_cout_buf = nullptr;
	}

private:
	streambuf* original_cout_buf = nullptr;
};