#pragma once
#include "FileManager.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class BufferManager : public FileManager {
public:
#ifdef __TEST__
    BufferManager() ;
#endif
    static BufferManager& getInstance() {
        static BufferManager instance;
        return instance;
    }

    virtual void writeBuffer(string command);
    virtual void removeBuffer(string command);
    virtual vector<string> readBuffer();
    virtual void flushBuffer();

    void mergeConsecutiveErases(std::vector<string>& eraseCmds);
    void updateErase(std::vector<string>& eraseCmds);

    string writeCommand(unsigned int line, string value) {
        return "W " + to_string(line) + " " + value;
    }
    string eraseCommand(unsigned int line, unsigned int size) {
        return "E " + to_string(line) + " " + to_string(size);
    }
    char getCmdType(string cmdString) {
        return cmdFormat.parseCommand(cmdString).type;
    }
    int getCmdLba(string cmdString) {
        return cmdFormat.parseCommand(cmdString).lba;
    }
    int getCmdSize(string cmdString) {
        return cmdFormat.parseCommand(cmdString).size;
    }
    string getCmdValue(string cmdString) {
        return cmdFormat.parseCommand(cmdString).value;
    }
    CommandFormat cmdFormat;
private:
#ifndef __TEST__
    BufferManager() ;
#endif
    BufferManager& operator=(const BufferManager& otherInstance) = delete;
    BufferManager(const BufferManager& otherInstance) = delete;
    ifstream getCommandBuffer();

    const int MAX_COMMAND_NUM_IN_BUFFER = 10;
    const string COMMAND_BUFFER_NAME = "buffer.txt";
};