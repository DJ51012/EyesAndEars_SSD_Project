#pragma once
#ifdef CREATEDLL_EXPORTS
#define SCENARIO_API __declspec(dllexport)
#else
#define SCENARIO_API __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <string>
#include "../TestShell/CommandSet.h"
#include "ScenarioCaller.h"


extern "C" SCENARIO_API ScenarioCaller* getScenarioCaller();
extern "C" SCENARIO_API bool isValidScenario(ScenarioCaller* sc, string scenarioName, vector<string> args);
extern "C" SCENARIO_API vector<CommandSet> callScenario(ScenarioCaller* sc, string scenarioName, vector<string> args);