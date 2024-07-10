#pragma once
#include <string>

#define interface struct 

#define FILE_NAME_RESULT "result.txt"
#define FILE_NAME_NAND "nand.txt"

using namespace std;

interface SsdDriver {
public:
    virtual void write(unsigned int lba_index, string value) = 0;
    virtual void read(unsigned int lba_index) = 0;
    virtual void setRedirection(bool redirection) = 0;
};