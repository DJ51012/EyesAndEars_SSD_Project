#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/SsdDriver.h"

class MockSsdDriver : public SsdDriver {
public:
	MOCK_METHOD(void, write, (unsigned int lba_index, string value), (override));
	MOCK_METHOD(void, read, (unsigned int lba_index), (override));
	MOCK_METHOD(void, erase, (unsigned int lba_index, unsigned int size), (override));
};