#pragma once

#include "FileIoInterface.h"

class RealFileIo : public FileIoInterface {
public:
	FILE* Open(const char* filename, const char* mode) override;
	size_t Read(int fd, void* buf, size_t count) override;
	size_t Write(const void* data, size_t size, size_t num, FILE* file) override;
	int Close(FILE* file) override;
};