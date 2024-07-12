#pragma once
#include "Logger.h"

void Logger::printLog(string logMsg, const char functionName[]) {
	string logEntry = getLogEntry(logMsg, functionName);
	// test scenario에서 cout 출력 buffer를 변경하므로 cout 대신 printf 사용
	printf(logEntry.c_str());	// cout << logEntry;
	try {
		logFileManager.recordLogOnFile(logEntry, timeManager.getFileNameForStore());
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

string Logger::getLogEntry(string logMsg, const char functionName[]) {
	return timeManager.getDateTimeForLog() + getOrganizedFunctionName(functionName) + logMsg + "\n";
}

string Logger::getOrganizedFunctionName(const char functionName[]) {
	string strFuncName(functionName);
	strFuncName.resize(LoggerConst::MAX_FUNCTION_NAME, ' ');
	return strFuncName;
}
