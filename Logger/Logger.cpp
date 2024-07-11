#pragma once
#include "Logger.h"
#include "TimeManager.cpp"

void Logger::print(string logMsg, const char functionName[]) {
	string log = timeManager.getDateTimeForLog() + getOrganizedFunctionName(functionName) + logMsg + "\n";
	cout << log;
	int logFileSize = getLogFileSize();
	if (logFileSize != -1 && logFileSize + log.size() + 1 > LoggerConst::MAX_LOG_FILE_SIZE) {
		changePrevFile();
	}
	fstream logFile = getLogFile();
	logFile << log;
	logFile.close();
}

string Logger::getOrganizedFunctionName(const char functionName[]) {
	string strFuncName(functionName);
	strFuncName.resize(50, ' ');
	return strFuncName;
}

void Logger::changePrevFile() {
	string zipFileName = "";
	string previousLogFile = findPrevLogFile();
	//if (previousLogFile.length() == 0)
		//PRINT("findPrevLogFile Failed");

	if (previousLogFile.length() && previousLogFile.rfind(LoggerConst::LOG_EXTENSION) != string::npos) {
		zipFileName = changeExtension(previousLogFile);

		if (rename(previousLogFile.c_str(), zipFileName.c_str()) != 0) {
			cout << "No previous Log File" << endl;
		}
	}

	previousLogFile = timeManager.getFileNameForStore();
	if (rename(LoggerConst::LATEST_LOG_FILE_NAME.c_str(), previousLogFile.c_str()) != 0) {
		cerr << "changePrevFile Failed" << endl;
	}
}

string Logger::changeExtension(const string previousLogFile) {
	return previousLogFile.substr(0, previousLogFile.find_last_of('.')) + LoggerConst::ZIP_EXTENSION;
}


fstream Logger::getLogFile() {
	checkFileExistence();
	fstream logFile(LoggerConst::LATEST_LOG_FILE_NAME, ios::app);
	return logFile;
}

void Logger::checkFileExistence() {
	ifstream logFileForCheck(LoggerConst::LATEST_LOG_FILE_NAME);
	if (logFileForCheck.is_open() == false) {
		createLogFile();
		return;
	}
	logFileForCheck.close();
}

void Logger::createLogFile() {
	ofstream logfile(LoggerConst::LATEST_LOG_FILE_NAME);
	if (logfile.is_open() == false)
		cout << "Create Log File Failed!" << endl;
	logfile.close();
}

int Logger::getLogFileSize()
{
	ifstream logFileForCheck(LoggerConst::LATEST_LOG_FILE_NAME);
	if (logFileForCheck.is_open() == false) return -1;
	logFileForCheck.close();

	struct stat statBuffer;
	if (stat(LoggerConst::LATEST_LOG_FILE_NAME.c_str(), &statBuffer) != 0) {
		cout << "Get Log File Size Failed!" << endl;
		return -1;
	}
	cout << "file size: " << statBuffer.st_size << endl;
	return statBuffer.st_size;
}

string Logger::findPrevLogFile() {
	filesystem::path currentPath = filesystem::current_path();
	if (std::filesystem::exists(currentPath) && std::filesystem::is_directory(currentPath)) {
		for (const auto& file : std::filesystem::directory_iterator(currentPath)) {
			if (file.is_regular_file() && file.path().extension() == LoggerConst::LOG_EXTENSION) {
				string strFileName = file.path().filename().string();
				if (strFileName.substr(0, LoggerConst::PREFIX_PREV_FILE.length()).compare(LoggerConst::PREFIX_PREV_FILE) == 0) {
					cout << "Found Log File: " << strFileName << endl;
					return strFileName;
				}
			}
		}
	}
	else
	{
		cout << "Invalid file path" << endl;
	}
	return "";
}
