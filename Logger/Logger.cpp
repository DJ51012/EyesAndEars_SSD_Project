#pragma once
#include "Logger.h"

void Logger::print(string logMsg, const char functionName[]) {
	string log = getDateTimeForLog() + getOrganizedFunctionName(functionName) + logMsg + "\n";
	cout << log;
	int logFileSize = getLogFileSize();
	if (logFileSize != -1 && logFileSize + log.size() + 1 > MAX_LOG_FILE_SIZE) {
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

	if (previousLogFile.length() && previousLogFile.rfind(LOG_EXTENSION) != string::npos) {
		zipFileName = changeExtension(previousLogFile);

		if (rename(previousLogFile.c_str(), zipFileName.c_str()) != 0) {
			cout << "No previous Log File" << endl;
		}
	}

	previousLogFile = getFileNameForStore();
	if (rename(LATEST_LOG_FILE_NAME.c_str(), previousLogFile.c_str()) != 0) {
		cerr << "changePrevFile Failed" << endl;
	}
}

string Logger::changeExtension(const string previousLogFile) {
	//return previousLogFile.substr(0, previousLogFile.length() - LOG_EXTENSION.length()) + ZIP_EXTENSION;
	return previousLogFile.substr(0, previousLogFile.find_last_of('.')) + ZIP_EXTENSION;
}

string Logger::getDateTimeForLog() {
	tm tmTime = getLocalTime();
	string ret = "[";
	ret += getYear(&tmTime) + ".";
	ret += getMonth(&tmTime) + ".";
	ret += getDay(&tmTime) + " ";
	ret += getHour(&tmTime) + ":";
	ret += getMinute(&tmTime) + ":";
	ret += getSecond(&tmTime) + "] ";
	return ret;
}

string Logger::getFileNameForStore() {
	tm tmTime = getLocalTime();
	string ret = PREFIX_PREV_FILE;
	ret += getYear(&tmTime);
	ret += getMonth(&tmTime);
	ret += getDay(&tmTime) + "_";
	ret += getHour(&tmTime) + "h_";
	ret += getMinute(&tmTime) + "m_";
	ret += getSecond(&tmTime) + "s";
	ret += LOG_EXTENSION;
	return ret;
}

tm Logger::getLocalTime() {
	time_t tm_t = time(nullptr);
	tm tmTime;
	localtime_s(&tmTime, &tm_t);
	return tmTime;
}

string Logger::toStringWithZeroPadding(int number)
{
	string str = to_string(number);
	if (str.length() < 2)
		str.insert(0, 2 - str.length(), '0');
	return str;
}

string Logger::getYear(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_year - 100);
}
string Logger::getMonth(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_mon + 1);
}
string Logger::getDay(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_mday);
}
string Logger::getHour(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_hour);
}
string Logger::getMinute(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_min);
}
string Logger::getSecond(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_sec);
}

fstream Logger::getLogFile() {
	checkFileExistence();
	fstream logFile(LATEST_LOG_FILE_NAME, ios::app);
	return logFile;
}

void Logger::checkFileExistence() {
	ifstream logFileForCheck(LATEST_LOG_FILE_NAME);
	if (logFileForCheck.is_open() == false) {
		createLogFile();
		return;
	}
	logFileForCheck.close();
}

void Logger::createLogFile() {
	ofstream logfile(LATEST_LOG_FILE_NAME);
	if (logfile.is_open() == false)
		cout << "Create Log File Failed!" << endl;
	logfile.close();
}

int Logger::getLogFileSize()
{
	ifstream logFileForCheck(LATEST_LOG_FILE_NAME);
	if (logFileForCheck.is_open() == false) return -1;
	logFileForCheck.close();

	struct stat statBuffer;
	if (stat(LATEST_LOG_FILE_NAME.c_str(), &statBuffer) != 0) {
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
			if (file.is_regular_file() && file.path().extension() == LOG_EXTENSION) {
				string strFileName = file.path().filename().string();
				if (strFileName.substr(0, PREFIX_PREV_FILE.length()).compare(PREFIX_PREV_FILE) == 0) {
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
