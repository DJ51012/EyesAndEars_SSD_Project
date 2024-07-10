#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdio.h>

#include "../TestShell/FileIoInterface.h"


class MockFileIO : public FileIoInterface {
public:
    MOCK_METHOD(FILE* , Open, (const char* filename, const char* mode), (override));
    MOCK_METHOD(size_t, Read, (int fd, void* buf, size_t count), (override));
    MOCK_METHOD(size_t, Write, (const void* data,
        size_t size, size_t num, FILE* file), (override));
    MOCK_METHOD(int , Close, (FILE* file), (override));
};