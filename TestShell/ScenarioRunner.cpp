#include "ScenarioRunner.h"

bool ScenarioRunner::runScenario(vector<CommandSet> cmdList)
{
	for (auto cmdSet : cmdList)
	{
		cout << "RUN Scenario !!!" << endl;
		if (!runCommandSet(cmdSet))
			return false;
	}
	return true;
}

bool ScenarioRunner::runCommandSet(CommandSet cmd)
{
	bool ret = true;
	CommandSetRunnerInterface* csr = getCommandRunner(cmd);
	if (csr == nullptr)
		return false;
	try {
		ret = csr->runCommandSet();
	}
	catch (exception& e) {
		cout << "ERROR: " << e.what() << endl;
		return false;
	}

	return ret;
}

CommandSetRunnerInterface* ScenarioRunner::getCommandRunner(CommandSet cmd)
{
	if (cmd.strCmd == TEST_CMD::WRITE) return new WriteRunner(cmd);
	if (cmd.strCmd == TEST_CMD::READ) return  new ReadRunner(cmd);
	if (cmd.strCmd == TEST_CMD::FULLWRITE) return  new FullWriteRunner(cmd);
	if (cmd.strCmd == TEST_CMD::FULLREAD) return  new FullReadRunner(cmd);

	return nullptr;
}


