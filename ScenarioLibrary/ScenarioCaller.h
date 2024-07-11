#pragma once
#include "../TestShell/CommandSet.h"
#include "Scenario.h"

class ScenarioCaller
{
public:
	ScenarioCaller();
	bool isValidScenario(string scenarioName, vector<string> args);
	vector<CommandSet> callScenario(string scenarioName, vector<string> args);

private:
	ScenarioInterface* getScenario(string scenarioName);
	vector<string> m_scenarioList;
};
