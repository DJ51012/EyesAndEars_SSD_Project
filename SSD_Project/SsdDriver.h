#define interface struct 
#include <iostream>
using namespace std;

interface SsdDriver {
    virtual void write(unsigned int lba_index, unsigned int value) = 0;
    virtual string read(unsigned int lba_index) = 0;
};  
