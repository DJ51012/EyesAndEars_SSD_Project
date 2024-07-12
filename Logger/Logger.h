#pragma once
#include <iostream>
#include <string>
#include "TimeManager.h"
#include "LogFileManager.h"

using namespace std;

#ifdef __TEST__
#define ENABLE_LOG	(0)
#else
#define ENABLE_LOG	(1)
#endif

#if (ENABLE_LOG)
#define PRINT_LOG(logMsg) (Logger::getInstance().printLog(logMsg, __FUNCTION__))
#else
#define PRINT_LOG(logMsg)
#endif

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void printLog(string logMsg, const char functionName[]);

private:
	Logger(){}
	Logger& operator=(const Logger& otherInstance) = delete;
	Logger(const Logger& otherInstance) = delete;

	string getLogEntry(string logMsg, const char functionName[]);
	string getOrganizedFunctionName(const char functionName[]);

	TimeManager timeManager;
	LogFileManager logFileManager;
};

