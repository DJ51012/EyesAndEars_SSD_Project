#pragma once

#include <string>

class LoggerConst {
public:
	static const int MAX_LOG_FILE_SIZE = 10 * 1024;
	static const std::string LATEST_LOG_FILE_NAME;
	static const std::string LOG_EXTENSION;
	static const std::string ZIP_EXTENSION;
	static const std::string PREFIX_PREV_FILE;
};

const std::string LoggerConst::LATEST_LOG_FILE_NAME = "latest.log";
const std::string LoggerConst::LOG_EXTENSION = ".log";
const std::string LoggerConst::ZIP_EXTENSION = ".zip";
const std::string LoggerConst::PREFIX_PREV_FILE = "until_";