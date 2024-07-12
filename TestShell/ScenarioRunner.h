#pragma once
#include "CommandSetRunner.h"

class ScenarioRunner
{
public:
	bool runScenario(vector<CommandSet> cmdList);
	bool runCommandSet(CommandSet cmd);
	CommandSetRunnerInterface* getCommandRunner(CommandSet cmd);

private:
};