#include "RealSsdDriver.h"
#include "RealFileIo.h"
#include "testshell.h"

#include "../ScenarioLibrary/ScenarioLibrary.h"
#pragma comment(lib, "../x64/Debug/ScenarioLibrary.lib")

int main() {
	RealSsdDriver rsd;
	RealFileIo rfi;
	TestShell ts{ "", {}, &rsd, &rfi };
	ts.start_shell();
}