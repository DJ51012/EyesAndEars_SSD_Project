#pragma once
#include <string>
#include <ctime>
#include "LoggerConst.h"

using namespace std;

class TimeManager {
public:
	string getDateTimeForLog();
	string getFileNameForStore();
private:
	tm getLocalTime();
	string toStringWithZeroPadding(int number);
	string getYear(tm* tmTime);
	string getMonth(tm* tmTime);
	string getDay(tm* tmTime);
	string getHour(tm* tmTime);
	string getMinute(tm* tmTime);
	string getSecond(tm* tmTime);
};