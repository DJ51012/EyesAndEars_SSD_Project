#include "CommandManager.h"
#include "Ssd.cpp"

int main(int argc, char* argv[]) {
    Ssd ssd;
    CommandManager cm;

    try {
        cm.doSsdCommand(&ssd, argc, argv);
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
        return -1;
    }

    return 0;
}