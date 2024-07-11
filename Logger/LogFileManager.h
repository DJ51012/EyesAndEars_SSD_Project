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
	bool checkExceedFileSize(const string log, int logFileSize);
	bool IsLatestLogFileExist();
	void createLogFile();
	int getLogFileSize();
	void changePrevFile(const string fileNameWithTime);
	bool IsExistPrevLogFile(const string prevLogFile);
	string changeExtension(const string previousLogFile);
	void changeFileName(const string src, const string dest);
	string findPrevLogFile();
	bool IsCorrectDirectory(const filesystem::path& currentPath);
	bool IsPrevLogFile(const filesystem::directory_entry& file);
};