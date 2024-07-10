#pragma once
#include "Logger.h"

void Logger::print(string logMsg, const char functionName[]) {
	string log = getDateTime() + getOrganizedFunctionName(functionName) + logMsg + "\n";
	cout << log;
}

string Logger::getOrganizedFunctionName(const char functionName[]) {
	string strFuncName(functionName);
	strFuncName.resize(30, ' ');
	return strFuncName;
}

time_t Logger::getNow() {
	return time(nullptr);
}

string Logger::toStringWithZeroPadding(int number)
{
	string str = to_string(number);
	if (str.length() < 2)
		str.insert(0, 2 - str.length(), '0');
	return str;
}

string Logger::getDateTime() {
	time_t tm_t = getNow();
	string ret = "[";
	tm* tmTime = localtime(&tm_t);
	ret += toStringWithZeroPadding(tmTime->tm_year + (1900 - 2000)) + ".";
	ret += toStringWithZeroPadding(tmTime->tm_mon + 1) + ".";
	ret += toStringWithZeroPadding(tmTime->tm_mday) + " ";
	ret += toStringWithZeroPadding(tmTime->tm_hour) + ":";
	ret += toStringWithZeroPadding(tmTime->tm_min) + ":";
	ret += toStringWithZeroPadding(tmTime->tm_sec) + "] ";
	return ret;
}





