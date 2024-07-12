#include <iostream>
#include "Scenario.h"

using namespace std;

vector<CommandSet> TestApp1::getCommandSetList()
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

void TestApp1::setData(vector<string> args)
{
	if (args.size() <= NR_DATA)
		for (int i = 0; i < args.size(); i++)
		{
			m_Data[i] = args[i];
		}
}

vector<CommandSet> TestApp2::getCommandSetList()
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

void TestApp2::setData(vector<string> args)
{
	if (args.size() <= NR_DATA) {
		for (int i = 0; i < args.size(); i++)
		{
			m_Data[i] = args[i];
		}
	}
}


vector<CommandSet> WriteReadCompareLoop50::getCommandSetList()
{
	vector<CommandSet> cmdSetList = {};

	for (int i = 0; i < NR_LOOP; i++) {
		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("write")
			.setStartLba(0)
			.setLastLba(5)
			.setIteration(1)
			.setData(m_Data[i % m_nrData])
			.returnCommandSet()
		);

		cmdSetList.push_back(
			CommandSet::CommandSetBuilder()
			.setCommand("read")
			.setStartLba(0)
			.setLastLba(5)
			.setData(m_Data[i % m_nrData])
			.setExpectData(true)
			.returnCommandSet()
		);
	}

	return cmdSetList;
}

void WriteReadCompareLoop50::setData(vector<string> args)
{
	int m_nrData = args.size() < 2 ? 2 : (args.size() < NR_LOOP ? args.size() : NR_LOOP);

	if (args.size() <= NR_LOOP) {
		for (int i = 0; i < args.size(); i++)
		{
			m_Data[i] = args[i];
		}
	}
}