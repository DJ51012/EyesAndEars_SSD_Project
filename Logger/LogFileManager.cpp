#include "LogFileManager.h"

void LogFileManager::recordLogOnFile(const string log, const string fileNameWithTime) {
	if (checkExceedFileSize(log, getLogFileSize())) {
		changePrevFile(fileNameWithTime);
	}
	fstream logFile = getLogFile();
	logFile << log;
	logFile.close();
}

bool LogFileManager::checkExceedFileSize(const string log, int logFileSize) {
	return (logFileSize != -1 && logFileSize + log.size() + 1 > LoggerConst::MAX_LOG_FILE_SIZE);
}

void LogFileManager::changePrevFile(const string fileNameWithTime) {
	string previousLogFile = findPrevLogFile();

	if (IsExistPrevLogFile(previousLogFile)) {
		changeFileName(previousLogFile, changeExtension(previousLogFile));
	}
	changeFileName(LoggerConst::LATEST_LOG_FILE_NAME, fileNameWithTime);
}

bool LogFileManager::IsExistPrevLogFile(const string prevLogFile) {
	return (prevLogFile.length() && prevLogFile.rfind(LoggerConst::LOG_EXTENSION) != string::npos);
}

string LogFileManager::changeExtension(const string previousLogFile) {
	return previousLogFile.substr(0, previousLogFile.find_last_of('.')) + LoggerConst::ZIP_EXTENSION;
}

void LogFileManager::changeFileName(const string src, const string dest) {
	if (rename(src.c_str(), dest.c_str()) != 0) {
		cout << "Chanege File Name failed src: " << src << "dest: " << dest << endl;
	}
}

fstream LogFileManager::getLogFile() {
	if (IsLatestLogFileExist() == false)
		createLogFile();

	fstream logFile(LoggerConst::LATEST_LOG_FILE_NAME, ios::app);
	return logFile;
}

bool LogFileManager::IsLatestLogFileExist() {
	ifstream logFileForCheck(LoggerConst::LATEST_LOG_FILE_NAME);
	if (logFileForCheck.is_open() == false) {
		return false;
	}
	logFileForCheck.close();
	return true;
}

void LogFileManager::createLogFile() {
	ofstream logfile(LoggerConst::LATEST_LOG_FILE_NAME);
	if (logfile.is_open() == false) {
		cout << "Create Log File Failed!" << endl;
		return;
	}
	logfile.close();
}

int LogFileManager::getLogFileSize() {
	if (IsLatestLogFileExist() == false)
		return -1;

	struct stat statBuffer;
	if (stat(LoggerConst::LATEST_LOG_FILE_NAME.c_str(), &statBuffer) != 0) {
		cout << "Get Log File Size Failed!" << endl;
		return -1;
	}
	// cout << "file size: " << statBuffer.st_size << endl;
	return statBuffer.st_size;
}

string LogFileManager::findPrevLogFile() {
	filesystem::path currentPath = filesystem::current_path();
	if (IsCorrectDirectory(currentPath) == false) {
		cout << "Invalid file path" << endl;
		return "";
	}
	for (const auto& file : filesystem::directory_iterator(currentPath)) {
		if (IsPrevLogFile(file))
			return file.path().filename().string();
	}
	return "";
}

bool LogFileManager::IsCorrectDirectory(const filesystem::path& currentPath) {
	return (filesystem::exists(currentPath) && filesystem::is_directory(currentPath));
}

bool LogFileManager::IsPrevLogFile(const filesystem::directory_entry& file) {
	if (file.is_regular_file() == false)
		return false;
	if (file.path().extension().string().compare(LoggerConst::LOG_EXTENSION) != 0)
		return false;
	string strFileName = file.path().filename().string();
	if (strFileName.substr(0, LoggerConst::PREFIX_PREV_FILE.length()).compare(LoggerConst::PREFIX_PREV_FILE) != 0)
		return false;

	// cout << "Found Log File: " << strFileName << endl;
	return true;
}
