#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

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
	string getDateTimeForLog();
	string getFileNameForStore();
	string getYear(tm* tmTime);
	string getMonth(tm* tmTime);
	string getDay(tm* tmTime);
	string getHour(tm* tmTime);
	string getMinute(tm* tmTime);
	string getSecond(tm* tmTime);

	fstream getLogFile();
	void createLogFile();
	int getLogFileSize();
	void changePrevFile();
	string findPrevLogFile();

	const int MAX_LOG_FILE_SIZE = 10 * 1024;
	const string LATEST_LOG_FILE_NAME = "latest.log";
	const string LOG_EXTENSION = ".log";
	const string ZIP_EXTENSION = ".zip";

	//string previousLogFile = "";
};

