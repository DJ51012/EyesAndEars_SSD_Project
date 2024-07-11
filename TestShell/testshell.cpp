#include "testshell.h"

	TestShell::TestShell(string cmd, vector<string> args, SsdDriver* ssd_driver, FileIoInterface*  fio_interface)
		: cmd(cmd), args(args), ssd_Driver(ssd_driver), fio(fio_interface){
	
	}

	void TestShell::setDriver(SsdDriver* driver)  {
		this->ssd_Driver = driver;
	}

	void TestShell::setFileIo(FileIoInterface* fio) {
		this->fio = fio;
	}

	bool TestShell::run_cmd() {

		AssertWrongCmd();
		AssertWrongArguments();

		auto cmd_runner = get_test_cmd_runner();
		if (cmd_runner != nullptr) {
			cmd_runner->run_cmd(ssd_Driver, fio, args);
			delete cmd_runner;
			return true;
		}

		return false;
	}


	/*
	void set_user_input(const string& user_input) {
		istringstream user_input_stream{ user_input };
		string arg;
		bool isFirstArg = true;

		clearUserData();

		while (getline(user_input_stream, arg, ' ')) {
			if (arg.empty()) continue;

			if (isFirstArg) {
				this->cmd = arg;
				isFirstArg = false;
			}
			else {
				this->args.push_back(arg);
			}
		}
	}
	*/

	void TestShell::set_user_input(const string& user_input) {

		clearUserData();

		ParsedCommand pCmd = parseUserInput(user_input);
		cmd = pCmd.m_cmd;
		args = pCmd.m_args;
	}

	ParsedCommand TestShell::parseUserInput(string user_input)
	{
		ParsedCommand pCmd = {};
		istringstream user_input_stream{ user_input };
		string arg;
		bool isFirstArg = true;

		while (getline(user_input_stream, arg, ' ')) {
			if (arg.empty()) continue;

			if (isFirstArg) {
				pCmd.m_cmd = arg;
				isFirstArg = false;
			}
			else {
				pCmd.m_args.push_back(arg);
			}
		}
		return pCmd;
	}

	void TestShell::start_shell() {
		while (1) {
			ScenarioCaller* sc = getScenarioCaller();
			sc->test1();

			cout << "TestShell> ";
			string user_input;

			getline(std::cin, user_input);
			this->set_user_input(user_input);

			if (sc->isValidScenario(this->cmd, this->args))
			{
				ScenarioRunner* sr = new ScenarioRunner;
				sr->runScenario(sc->callScenario(this->cmd, this->args));
				break;
			}

			try {
				this->run_cmd();
			}
			catch (std::invalid_argument& e) {
				cout << e.what() << endl;
			}
			catch (std::runtime_error& e) {
				cout << e.what() << endl;
			}
		}
	}

	void TestShell::AssertWrongCmd()
	{
		auto allowed_cmds = {
			TEST_CMD::WRITE, TEST_CMD::READ, TEST_CMD::EXIT, TEST_CMD::HELP,
			TEST_CMD::FULLWRITE, TEST_CMD::FULLREAD
		};
		for (auto& cmd : allowed_cmds) {
			if (this->cmd == cmd) return;
		}

		throw invalid_argument("INVALID COMMAND");
	}

	void TestShell::AssertWrongArguments()
	{
		if (cmd == TEST_CMD::WRITE && args.size() >= 2 && isValidLbaIndex(args[0]) && isValidWriteValue(args[1])) return;
		if (cmd == TEST_CMD::READ && args.size() >= 1 && isValidLbaIndex(args[0])) return;
		if (cmd == TEST_CMD::FULLWRITE && args.size() >= 1 && isValidWriteValue(args[0])) return;
		if (cmd == TEST_CMD::FULLREAD && args.size() == 0 ) return;
		if (cmd == TEST_CMD::EXIT) return;
		if (cmd == TEST_CMD::HELP) return;
			
		throw invalid_argument("WRONG ARGUMENT");
	}

	bool TestShell::isValidLbaIndex(string& lba_index) {
		return stoi(lba_index) < 100;
	}

	bool TestShell::isValidWriteValue(string& write_value) {
		return write_value.length() == 10 && write_value.substr(0, 2) == "0x";
	}

	TestCmd* TestShell::get_test_cmd_runner() {
		if (cmd == TEST_CMD::WRITE) return new WriteTestCmd();
		if (cmd == TEST_CMD::READ) return new ReadTestCmd();
		if (cmd == TEST_CMD::EXIT) return new ExitTestCmd();
		if (cmd == TEST_CMD::HELP) return new HelpTestCmd();
		if (cmd == TEST_CMD::FULLWRITE) return new FullwriteTestCmd();
		if (cmd == TEST_CMD::FULLREAD) return new FullreadTestCmd();
		return nullptr;
	}

	void TestShell::clearUserData()
	{
		this->cmd = "";
		this->args.clear();
	}