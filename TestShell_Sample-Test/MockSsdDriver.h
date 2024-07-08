#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../SSD_Project/SsdDriver.h"

class MockSsdDriver : public SsdDriver {
public:
	MOCK_METHOD(void, write, (unsigned int lba_index, unsigned int value), (override));
	MOCK_METHOD(unsigned int, read, (unsigned int lba_index), (override));
};