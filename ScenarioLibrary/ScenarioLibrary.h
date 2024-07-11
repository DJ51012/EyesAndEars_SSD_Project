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
//#include "ScenarioBuilder.h"

#define interface struct

interface ScenarioBuilder
{
public:
	virtual vector<CommandSet> getCommandSetList() = 0;
	virtual void setData(vector<string> args) = 0;
};


class TestApp1 : public ScenarioBuilder
{
	//vector<CommandSet> getCommandSetList() override;
	//void setData(vector<string> args) override;

	vector<CommandSet> getCommandSetList() override
	{
		vector<CommandSet> cmdSetList = {};

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("fullwrite")
			.setData(m_Data[0])
			.returnCommandSet()
		);

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("fullread")
			.setData(m_Data[0])
			.setExpectData(true)
			.returnCommandSet()
		);

		return cmdSetList;
	}

	void setData(vector<string> args) override
	{
		if (args.size() <= NR_DATA)
			for (int i = 0; i < args.size(); i++)
			{
				m_Data[i] = args[i];
			}
	}

private:
	const int NR_DATA = 1;
	string m_Data[1] = { "0x12345678" };
};

class TestApp2 : public ScenarioBuilder
{
	vector<CommandSet> getCommandSetList() override
	{
		vector<CommandSet> cmdSetList = {};

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("write")
			.setStartLba(0)
			.setLastLba(5)
			.setIteration(50)
			.setData(m_Data[0])
			.returnCommandSet()
		);

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("write")
			.setStartLba(0)
			.setLastLba(5)
			.setData(m_Data[1])
			.returnCommandSet()
		);

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("read")
			.setStartLba(0)
			.setLastLba(5)
			.setData(m_Data[1])
			.setExpectData(true)
			.returnCommandSet()
		);

		return cmdSetList;
	}

	void setData(vector<string> args) override
	{
		if (args.size() <= NR_DATA) {
			for (int i = 0; i < args.size(); i++)
			{
				m_Data[i] = args[i];
			}
		}
	}

private:
	const int NR_DATA = 2;
	string m_Data[2] = { "0xAAAABBBB", "0x12345678" };
};


using namespace std;











//====================


namespace SCENARIO_NAME {
	const string TESTAPP1 = "testapp1";
	const string TESTAPP2 = "testapp2";
};

class ScenarioCaller
{
public:
	ScenarioCaller()
	{
		m_scenarioList.clear();
		m_scenarioList.push_back(SCENARIO_NAME::TESTAPP1);
		m_scenarioList.push_back(SCENARIO_NAME::TESTAPP2);
	}
	void test1()
	{
		cout << "ScenarioCaller" << endl;
	};

	bool isValidScenario(string scenarioName, vector<string> args)
	{
		if (find(m_scenarioList.begin(), m_scenarioList.end(), scenarioName) == m_scenarioList.end())
			return false;
		return true;
	};

	vector<CommandSet> callScenario(string scenarioName, vector<string> args)
	{
		ScenarioBuilder* scenario = getScenario(scenarioName);
		scenario->setData(args);
		// If Have Data
		return scenario->getCommandSetList();
	};

	ScenarioBuilder* getScenario(string scenarioName) {

		if (scenarioName == SCENARIO_NAME::TESTAPP1) return new TestApp1();
		if (scenarioName == SCENARIO_NAME::TESTAPP2) return new TestApp2();

		return nullptr;
	}
private:
	vector<string> m_scenarioList;
};

extern "C" SCENARIO_API void Test1(int a);
extern "C" SCENARIO_API ScenarioCaller* getScenarioCaller();