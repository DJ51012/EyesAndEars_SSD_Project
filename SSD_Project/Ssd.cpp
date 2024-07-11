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
            Command cmd =  cmdFormat.parseCommand(cmdString);
            
            if (cmd.type == 'W' && cmd.lba == line) {
                fileManager->writeResult(cmd.value);
                return;
            }
        }
        fileManager->readNand(line);
    }

    void flush() {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Command cmd = cmdFormat.parseCommand(cmdString);
            switch (cmd.type) {
            case 'R' :
                fileManager->readNand(cmd.lba);
                break;
            case 'W':
                fileManager->writeNand(cmd.lba, cmd.value);
                break;
            case 'E':
                // erase 함수가 command buffer를 타게 수정될 수 있으므로 
                // erase 함수 호출하지 않고 erase 함수 내용 그대로 수행
                for (unsigned int offset = 0; offset < cmd.size; offset++) {
                    fileManager->writeNand((cmd.lba + offset), DEFAULT_VALUE);
                }
                break;
            default:
                break;
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