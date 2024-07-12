#pragma once
#include "ScenarioLibrary.h"

ScenarioCaller* getScenarioCaller()
{
	return new ScenarioCaller;
}

bool isValidScenario(ScenarioCaller* sc, string scenarioName, vector<string> args)
{
	return sc->isValidScenario(scenarioName, args);
}

vector<CommandSet> callScenario(ScenarioCaller* sc, string scenarioName, vector<string> args)
{
	return sc->callScenario(scenarioName, args);
}