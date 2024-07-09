#define interface struct 
#include <iostream>
using namespace std;

interface SsdDriver {
    virtual void write(unsigned int lba_index, string value) = 0;
    virtual string read(unsigned int lba_index) = 0;
};  
