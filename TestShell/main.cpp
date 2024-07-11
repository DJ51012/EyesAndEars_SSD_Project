#include "RealSsdDriver.h"
#include "RealFileIo.h"
#include "testshell.cpp"
#include "Runner.cpp"

#include "../Logger/TimeManager.cpp"
#include "../Logger/LogFileManager.cpp"
#include "../Logger/Logger.cpp"

int main(int argc, char* argv[]) {
	RealSsdDriver rsd;
	RealFileIo rfi;
	TestShell ts{ "", {}, &rsd, &rfi };

	if (argc == 1) {
		ts.start_shell();
	}
	else if(argc == 2){
		Runner runner;
		runner.start_runner(&ts, argv[1]);
	}
}