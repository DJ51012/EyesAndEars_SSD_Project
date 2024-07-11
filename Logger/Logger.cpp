#pragma once
#include "Logger.h"

void Logger::printLog(string logMsg, const char functionName[]) {
	string logEntry = getLogEntry(logMsg, functionName);
	cout << logEntry;
	logFileManager.recordLogOnFile(logEntry, timeManager.getFileNameForStore());
}

string Logger::getLogEntry(string logMsg, const char functionName[]) {
	return timeManager.getDateTimeForLog() + getOrganizedFunctionName(functionName) + logMsg + "\n";
}

string Logger::getOrganizedFunctionName(const char functionName[]) {
	string strFuncName(functionName);
	strFuncName.resize(LoggerConst::MAX_FUNCTION_NAME, ' ');
	return strFuncName;
}
