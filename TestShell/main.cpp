#include "RealSsdDriver.h"
#include "RealFileIo.h"
#include "testshell.cpp"
#include "Runner.cpp"

#include "../Logger/TimeManager.cpp"
#include "../Logger/LogFileManager.cpp"
#include "../Logger/Logger.cpp"

#include "../ScenarioLibrary/ScenarioLibrary.h"
#pragma comment(lib, "../x64/Debug/ScenarioLibrary.lib")

int main(int argc, char* argv[]) {
	RealSsdDriver rsd;
	RealFileIo rfi;
	TestShell ts{ "", {}, &rsd, &rfi };
	ts.start_shell();

	if (argc == 1) {
		ts.start_shell();
	}
	else if(argc == 2){
		Runner runner;
		runner.start_runner(&ts, argv[1]);
	}
}
