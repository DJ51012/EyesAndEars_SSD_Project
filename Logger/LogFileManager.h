#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "LoggerConst.h"

using namespace std;

class LogFileManager {
public:
	void recordLogOnFile(const string logMsg, const string fileNameWithTime);
private:
	fstream getLogFile();
	void checkFileExistence();
	void createLogFile();
	int getLogFileSize();
	void changePrevFile(const string fileNameWithTime);
	string findPrevLogFile();
	string changeExtension(const string previousLogFile);
};