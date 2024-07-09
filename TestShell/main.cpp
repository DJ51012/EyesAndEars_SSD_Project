#include "RealSsdDriver.h"
#include "RealFileIo.h"
#include "testshell.cpp"

int main() {
	RealSsdDriver rsd;
	RealFileIo rfi;
	TestShell ts{ "", {}, &rsd, &rfi };

	ts.start_shell();
}