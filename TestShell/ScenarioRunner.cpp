#include "ScenarioRunner.h"

bool ScenarioRunner::runScenario(vector<CommandSet> cmdList)
{
	for (auto cmdSet : cmdList)
	{
		if (!runCommandSet(cmdSet)) {
			return false;
		}
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
	catch (...) {
		cout << "EXCEPTION !" << endl;
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
	if (cmd.strCmd == TEST_CMD::ERASE) return  new EraseRunner(cmd);
	if (cmd.strCmd == TEST_CMD::ERASERANGE) return  new EraseRangeRunner(cmd);
	if (cmd.strCmd == TEST_CMD::FLUSH) return  new FlushRunner(cmd);

	return nullptr;
}


