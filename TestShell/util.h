#pragma once
#include<iostream>
#include<sstream>

using namespace std;

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