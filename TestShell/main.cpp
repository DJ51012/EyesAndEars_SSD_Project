#include "RealSsdDriver.h"
#include "testshell.cpp"

int main() {
	RealSsdDriver rsd;
	TestShell ts{ "", {}, &rsd, nullptr };

	ts.start_shell();
}