#include "SsdDriver.h"

class RealSsdDriver : public SsdDriver {
public:
	void write(unsigned int lba_index, string value) override;
	void read(unsigned int lba_index) override;
};