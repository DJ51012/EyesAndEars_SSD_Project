#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;
#define interface struct

struct CommandSet {
public:
	class CommandSetBuilder {
	public:
		CommandSetBuilder& setCommand(string command) {
			strCmd = command;
			return *this;
		};
		CommandSetBuilder& setStartLba(int startLba) {
			nStartLba = startLba;
			return *this;
		};
		CommandSetBuilder& setLastLba(int lastLba) {
			nLastLba = lastLba;
			return *this;
		};
		CommandSetBuilder& setData(string data) {
			strData = data;
			return *this;
		};
		CommandSetBuilder& setExpectData(bool expect) {
			bExpectData = expect;
			return *this;
		};

		CommandSetBuilder& setIteration(int iteration) {
			nIteration = iteration;
			return *this;
		};

		CommandSet returnCommandSet()
		{
			return CommandSet(strCmd,
				nStartLba,
				nLastLba,
				strData,
				bExpectData,
				nIteration);
		}

	private:
		string strCmd = "";
		int nStartLba = 0;
		int nLastLba = 0;
		string strData = "";
		bool bExpectData = false;
		int nIteration = 1;
	};

	string strCmd;
	int nStartLba;
	int nLastLba;
	string strData;
	bool bExpectData;
	int nIteration;

private:
	CommandSet(string strCmd,
		int nStartLba,
		int nLastLba,
		string strData,
		bool bExpectData,
		int nIteration)

		:strCmd{ strCmd },
		nStartLba{ nStartLba },
		nLastLba{ nLastLba },
		strData{ strData },
		bExpectData{ bExpectData },
		nIteration{ nIteration }
	{}
};