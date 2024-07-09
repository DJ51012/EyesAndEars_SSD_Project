#pragma once

#define interface struct 

#define FILE_NAME_RESULT "result.txt"
interface SsdDriver {
public:
    virtual void write(unsigned int lba_index, unsigned int value) = 0;
    virtual unsigned int read(unsigned int lba_index) = 0;
};