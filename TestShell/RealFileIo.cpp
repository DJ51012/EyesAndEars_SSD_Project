#include <stdexcept>
#include "RealFileIo.h"

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
    FILE* file = _fdopen(fd, "r");
    if (file == nullptr) {
        throw runtime_error("failed to load file");
    }

    fgets((char*)buf, count, file);
    fclose(file);

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
