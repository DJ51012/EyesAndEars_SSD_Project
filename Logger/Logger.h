#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "TimeManager.h"

using namespace std;

#define PRINT_LOG(logMsg) (Logger::getInstance().print(logMsg, __FUNCTION__))

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

	fstream getLogFile();
	void checkFileExistence();
	void createLogFile();
	int getLogFileSize();
	void changePrevFile();
	string findPrevLogFile();
	string changeExtension(const string previousLogFile);

	TimeManager timeManager;
};

