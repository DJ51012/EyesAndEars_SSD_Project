#pragma once
#include "ScenarioLibrary.h"

void Test1(int a)
{
	cout << "HEY *** NEW RETEST1 : " << a << endl;
}

ScenarioCaller* getScenarioCaller()
{
	return new ScenarioCaller;
}