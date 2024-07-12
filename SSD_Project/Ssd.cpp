#pragma once
#include "SsdDriver.h"
#include "FileManager.h"
#include "BufferManager.h"
#include "../Logger/Logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Ssd : public SsdDriver {
public:
    Ssd() {
        fileManager = &FileManager::getInstance();
        bufferManager = &BufferManager::getInstance();
    }

    void write(unsigned int line, string value) override {
        string writeCmdString = bufferManager->writeCommand(line, value);
        vector<string> cmdStrings = bufferManager->readBuffer();
        vector<string> eraseCmds;
        for (auto cmdString : cmdStrings) {
            char cmdType = bufferManager->getCmdType(cmdString);
            int lba = bufferManager->getCmdLba(cmdString); 
            if (cmdString == writeCmdString)
                return;
            if (cmdType == 'W' && lba == line) {
                bufferManager->removeBuffer(cmdString);
            }
            else if (cmdType == 'E') {
                eraseCmds.push_back(cmdString);
            }
        }
        if (cmdStrings.size() >= MAX_COMMAND_NUM_IN_BUFFER)
            flush();
        bufferManager->writeBuffer(writeCmdString);
        bufferManager->updateErase(eraseCmds);
    }

    void read(unsigned int line) override {
        vector<string> cmdStrings = bufferManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            char cmdType = bufferManager->getCmdType(cmdString);
            int lba = bufferManager->getCmdLba(cmdString);
            if (cmdType == 'W' && lba== line) {
                string value = bufferManager->getCmdValue(cmdString);
                fileManager->writeResult(value);
                PRINT_LOG(("Buffer Hit Read succeeded / LBA: " + to_string(line) + " / Value: " + value));
                return;
            }
            else if (cmdType == 'E') {
                int size = bufferManager->getCmdSize(cmdString);
                if (line >= lba && line < lba + size) {
                    fileManager->writeResult(DEFAULT_VALUE);
                    return;
                }
            }
        }
        fileManager->readNand(line);
    }

    void erase(unsigned int line, unsigned int size) override {
        size = min(MAX_ERASE_SIZE, size);
        vector<string> cmdStrings = bufferManager->readBuffer();
        vector<string> eraseCmds;
        for (auto cmdString : cmdStrings) {
            if (bufferManager->getCmdType(cmdString) == 'W') {
                if (bufferManager->getCmdLba(cmdString) >= line
                    && bufferManager->getCmdLba(cmdString)< line + size) {
                    bufferManager->removeBuffer(cmdString);
                }
            }
            else if (bufferManager->getCmdType(cmdString) == 'E') {
                if (bufferManager->getCmdLba(cmdString) == line 
                    && bufferManager->getCmdSize(cmdString) == size)
                    return;
                eraseCmds.push_back(cmdString);
            }
        }
        if (bufferManager->readBuffer().size() >= MAX_COMMAND_NUM_IN_BUFFER)
            flush();
        string eraseCmdString = bufferManager->eraseCommand(line, size);
        bufferManager->writeBuffer(eraseCmdString);

        eraseCmds.push_back(eraseCmdString);
        bufferManager->mergeConsecutiveErases(eraseCmds);
    }

    void flush() {
        PRINT_LOG(("Flush Start"));
        vector<string> cmdStrings = bufferManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            int lba = bufferManager->getCmdLba(cmdString);
            string value = "";
            int size;
            switch (bufferManager->getCmdType(cmdString)) {
            case 'R':
                fileManager->readNand(lba);
                break;
            case 'W':
                value = bufferManager->getCmdValue(cmdString);
                fileManager->writeNand(lba, value);
                break;
            case 'E':
                size = bufferManager->getCmdSize(cmdString);
                for (unsigned int offset = 0; offset < size; offset++) {
                    fileManager->writeNand((lba + offset), DEFAULT_VALUE);
                }
                break;
            default:
                break;
            }
        }
        bufferManager->flushBuffer();
        PRINT_LOG(("Flush Finished"));
    }
    FileManager* fileManager;
    BufferManager* bufferManager;
private:
    const string DEFAULT_VALUE = "0x00000000";
    const unsigned int MAX_COMMAND_NUM_IN_BUFFER = 10;
    const unsigned int MAX_ERASE_SIZE = 10;
};