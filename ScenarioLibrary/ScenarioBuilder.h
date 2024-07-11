#pragma once
#include <vector>
#include <string>
#include "../TestShell/CommandSet.h"

#define interface struct

interface ScenarioBuilder
{
public:
	virtual vector<CommandSet> getCommandSetList() = 0;
	virtual void setData(vector<string> args) = 0;
};


class TestApp1 : public ScenarioBuilder
{
	vector<CommandSet> getCommandSetList() override;
	void setData(vector<string> args) override;

private:
	const int NR_DATA = 1;
	string m_Data[1] = { "0x12345678" };
};

class TestApp2 : public ScenarioBuilder
{
	vector<CommandSet> getCommandSetList() override;
	void setData(vector<string> args) override;

private:
	const int NR_DATA = 2;
	string m_Data[2] = { "0xAAAABBBB", "0x12345678" };
};