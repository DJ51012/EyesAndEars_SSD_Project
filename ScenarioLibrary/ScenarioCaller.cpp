#include "ScenarioLibrary.h"
#include "ScenarioBuilder.h"


	ScenarioCaller::ScenarioCaller()
	{
		m_scenarioList.clear();
		m_scenarioList.push_back(SCENARIO_NAME::TESTAPP1);
		m_scenarioList.push_back(SCENARIO_NAME::TESTAPP2);
		m_scenarioList.push_back(SCENARIO_NAME::WRRDCMP50);
	}

	bool ScenarioCaller::isValidScenario(string scenarioName, vector<string> args)
	{
		if (find(m_scenarioList.begin(), m_scenarioList.end(), scenarioName) == m_scenarioList.end())
			return false;
		return true;
	};

	vector<CommandSet> ScenarioCaller::callScenario(string scenarioName, vector<string> args)
	{
		ScenarioBuilder* scenario = getScenario(scenarioName);
		scenario->setData(args);
		// If Have Data
		return scenario->getCommandSetList();
	};

	ScenarioBuilder* ScenarioCaller::getScenario(string scenarioName) {

		if (scenarioName == SCENARIO_NAME::TESTAPP1) return new TestApp1();
		if (scenarioName == SCENARIO_NAME::TESTAPP2) return new TestApp2();
		if (scenarioName == SCENARIO_NAME::WRRDCMP50) return new WriteReadCompareLoop50();

		return nullptr;
	}