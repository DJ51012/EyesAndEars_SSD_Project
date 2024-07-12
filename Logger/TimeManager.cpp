#include "TimeManager.h"

string TimeManager::getDateTimeForLog() {
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

string TimeManager::getFileNameForStore() {
	tm tmTime = getLocalTime();
	string ret = LoggerConst::PREFIX_PREV_FILE;
	ret += getYear(&tmTime);
	ret += getMonth(&tmTime);
	ret += getDay(&tmTime) + "_";
	ret += getHour(&tmTime) + "h_";
	ret += getMinute(&tmTime) + "m_";
	ret += getSecond(&tmTime) + "s";
	ret += LoggerConst::LOG_EXTENSION;
	return ret;
}

tm TimeManager::getLocalTime() {
	time_t tm_t = time(nullptr);
	tm tmTime;
	localtime_s(&tmTime, &tm_t);
	return tmTime;
}

string TimeManager::toStringWithZeroPadding(int number)
{
	string str = to_string(number);
	if (str.length() < 2)
		str.insert(0, 2 - str.length(), '0');
	return str;
}

string TimeManager::getYear(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_year - 100);
}
string TimeManager::getMonth(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_mon + 1);
}
string TimeManager::getDay(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_mday);
}
string TimeManager::getHour(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_hour);
}
string TimeManager::getMinute(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_min);
}
string TimeManager::getSecond(tm* tmTime) {
	return toStringWithZeroPadding(tmTime->tm_sec);
}