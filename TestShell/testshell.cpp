#include <string>
#include "../SSD_Project/SsdDriver.h"
#include "../TestShell/FileIOInterface.h"

using namespace std;

class TestShell {
public:
	TestShell(string cmd) : cmd(cmd) {

	}
	void setDriver(SsdDriver* driver)  {
		this->driver = driver;
	}
	void setFileIo(FileIoInterface* fio) {
		this->fio = fio;
	}

	bool run_cmd() {
		return false;
	}

private:
	string cmd;
	SsdDriver* driver;
	FileIoInterface* fio;

};