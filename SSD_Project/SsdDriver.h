#pragma once 
#include <string>

using namespace std;

#define interface struct

#define FILE_NAME_RESULT "result.txt"

interface SsdDriver {
public:
    virtual void write(unsigned int lba_index, string value) = 0;
    virtual void read(unsigned int lba_index) = 0;
};
