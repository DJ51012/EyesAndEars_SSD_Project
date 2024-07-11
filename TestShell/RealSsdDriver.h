#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
public:
	void write(unsigned int lba_index, string value) override;
	void read(unsigned int lba_index) override;
	void setRedirection(bool redirection) override;
	void erase(unsigned int lba_index, unsigned int size) override;
	void flush() override;

private:
	void execute_cmd(string& execute_cmd, string error_msg);
	bool isRedirection = false;
};