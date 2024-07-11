#pragma once
#include "CommandSet.h"
#include "testcmd.h"
#include "RealSsdDriver.h"
#include "RealFileIo.h"

#define interface struct

interface CommandSetRunnerInterface
{
public:
	CommandSetRunnerInterface(CommandSet cs) 
		: m_cs{ cs }
	{
	};
	virtual bool runCommandSet() = 0;

protected:
	CommandSet m_cs;

	SsdDriver* ssdDriver = new RealSsdDriver();
	FileIoInterface* fio = new RealFileIo();
};

class WriteRunner : public CommandSetRunnerInterface
{
public:
	WriteRunner(CommandSet cs) : CommandSetRunnerInterface (cs)
	{
		m_cmdRunner = new WriteTestCmd();
	};

	bool runCommandSet() override
	{
		vector<string> m_args;

		for (int iter = 0; iter < m_cs.nIteration; iter++)
		for (int lba = m_cs.nStartLba; lba <= m_cs.nLastLba; lba++)
		{
			m_args.clear();
			m_args.push_back(to_string(lba));
			m_args.push_back(m_cs.strData);
			m_cmdRunner->run_cmd(ssdDriver, fio, m_args);
		}
		return true;
	}

private:
	TestCmd* m_cmdRunner;
};

class ReadRunner : public CommandSetRunnerInterface
{
public:
	ReadRunner(CommandSet cs) : CommandSetRunnerInterface(cs)
	{
		m_cmdRunner = new ReadTestCmd();
	}

	bool runCommandSet() override
	{
		vector<string> m_args;

		for (int iter = 0 ;iter < m_cs.nIteration; iter++)
		for (int lba = m_cs.nStartLba; lba <= m_cs.nLastLba; lba++)
		{
			std_buf_util.change_stdout(&test_out_stream);

			m_args.clear();
			m_args.push_back(to_string(lba));
			m_cmdRunner->run_cmd(ssdDriver, fio, m_args);

			std_buf_util.restore_stdout();

			auto test_output = test_out_stream.str();

			if (m_cs.bExpectData && !compareResultData(test_output))
			{
					return false;
			}
		}

		return true;
	}

	bool compareResultData(std::string& test_output)
	{
		if (test_output.substr(0, 10) != m_cs.strData) {
			// cout << test_output << endl;
			// throw std::runtime_error("fullread compare failed.");
			return false;
		}
		return true;
	}

private:
	TestCmd* m_cmdRunner;

	std::ostringstream test_out_stream;
	StdBufUtil std_buf_util;
};

class FullWriteRunner : public CommandSetRunnerInterface
{
public:
	FullWriteRunner(CommandSet cs) : CommandSetRunnerInterface(cs)
	{
		m_cmdRunner = new FullwriteTestCmd();
	}

	bool runCommandSet() override
	{
		vector<string> m_args;

		m_args.clear();
		m_args.push_back(m_cs.strData);

		for (int iter = 0; iter < m_cs.nIteration; iter++)
		{
			m_cmdRunner->run_cmd(ssdDriver, fio, m_args);
		}
		return true;
	}


private:
	TestCmd* m_cmdRunner;
};

class FullReadRunner: public CommandSetRunnerInterface
{
public:
	FullReadRunner(CommandSet cs) : CommandSetRunnerInterface(cs)
	{
		m_cmdRunner = new FullreadTestCmd();
	}

	bool runCommandSet() override
	{
		vector<string> m_args;

		m_args.clear();

		for (int iter = 0; iter < m_cs.nIteration; iter++)
		{
			std_buf_util.change_stdout(&test_out_stream);

			m_cmdRunner->run_cmd(ssdDriver, fio, m_args);

			std_buf_util.restore_stdout();

			auto test_output = test_out_stream.str();
			if (m_cs.bExpectData && !compareResultData(test_output))
			{
				return false;
			}
		}

		return true;
	}

	bool compareResultData(std::string& test_output)
	{
		for (size_t i = 0; i < 100; ++i) {
			if (test_output.substr(i * 10, 10) != m_cs.strData) {
				// cout << test_output << endl;
				// throw std::runtime_error("fullread compare failed.");
				return false;
			}
		}
		return true;
	}

private:
	TestCmd* m_cmdRunner;

	std::ostringstream test_out_stream;
	StdBufUtil std_buf_util;
};