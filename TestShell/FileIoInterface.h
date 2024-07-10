#pragma once
#include <stdio.h>

class FileIoInterface {
public:
    virtual FILE* Open(const char* filename, const char* mode) = 0;
    virtual size_t Read(int fd, void* buf, size_t count) = 0;
    virtual size_t Write(const void* data, size_t size, size_t num, FILE* file) = 0;
    virtual int Close(FILE* file) = 0;
};