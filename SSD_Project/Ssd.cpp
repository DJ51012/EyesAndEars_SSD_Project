#pragma once
#include "SsdDriver.h"
#include "FileManager.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Ssd : public SsdDriver {
public:
    Ssd() {
        fileManager = &FileManager::getInstance();
    }

    void setFileManager(FileManager* newFileManager) {
        fileManager = newFileManager;
    }
    
    string writeCommand(unsigned int line, string value) {
        return "W " + to_string(line) + " " + value;
    }

    void write(unsigned int line, string value) override {
        vector<string> cmdStrings = fileManager->readBuffer();
        if (cmdStrings.size() >= 10)
            flush();
        fileManager->writeBuffer(writeCommand(line, value));
    }

    void read(unsigned int line) override {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Comamnd cmd =  cmdFormat.parseCommand(cmdString);
            
            if (cmd.RW == 'W' && cmd.lba == line) {
                fileManager->writeResult(cmd.value);
                return;
            }
        }
        fileManager->readNand(line);
    }

    void flush() {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Comamnd cmd = cmdFormat.parseCommand(cmdString);
            if (cmd.RW == 'R') {
                fileManager->readNand(cmd.lba);
            }
            else if (cmd.RW == 'W') {
                fileManager->writeNand(cmd.lba, cmd.value);
            }
        }
        fileManager->flushBuffer();
    }

    void erase(unsigned int line, unsigned int size) override {
        for (unsigned int offset = 0; offset < size; offset++) {
            fileManager->writeNand((line + offset), DEFAULT_VALUE);
        }
    }

    FileManager* fileManager;
    CommandFormat cmdFormat;

private:
    const string DEFAULT_VALUE = "0x00000000";
};