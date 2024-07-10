#pragma once
#include <iostream>
#include <string>

using namespace std;

#define PRINT(logMsg) (Logger::getInstance().print(logMsg, __FUNCTION__))

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void print(string logMsg, const char functionName[]);

private:
	Logger(){}
	Logger& operator=(const Logger& otherInstance) = delete;
	Logger(const Logger& otherInstance) = delete;
	string getOrganizedFunctionName(const char functionName[]);
	time_t getNow();
	string toStringWithZeroPadding(int number);
	string getDateTime();
};

