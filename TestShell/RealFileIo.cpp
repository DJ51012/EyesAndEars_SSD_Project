#include <stdexcept>
#include <io.h>
#include "RealFileIo.h"
#include "../Logger/Logger.h"

using namespace std;

FILE* RealFileIo::Open(const char* filename, const char* mode)
{
    FILE* file = nullptr;
    if (fopen_s(&file, filename, mode) != 0) {
        throw runtime_error("failed to open file");
    }

    return file;
}

size_t RealFileIo::Read(int fd, void* buf, size_t count)
{
    if (_read(fd, buf, (unsigned int)count) == -1) {
        return 0;
    }

    return count;
}

size_t RealFileIo::Write(const void* data, size_t size, size_t num, FILE* file)
{
    return 0; // Do not implement in the actual io scenario
}

int RealFileIo::Close(FILE* file)
{
    return fclose(file);
}
