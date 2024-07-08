#include <string>
using namespace std;
#define interface struct

interface SsdDriver {
    virtual void write(unsigned int lba_index, string value) = 0;
    virtual string read(unsigned int lba_index) = 0;
};