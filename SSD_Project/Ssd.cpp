#pragma once
#include "SsdDriver.h"
#include "FileManager.h"
#include "util.h"

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
    }

    void setFileManager(FileManager* newFileManager) {
        fileManager = newFileManager;
    }

    string writeCommand(unsigned int line, string value) {
        return "W " + to_string(line) + " " + value;
    }

    string eraseCommand(unsigned int line, unsigned int size) {
        return "E " + to_string(line) + " " + to_string(size);
    }

    void write(unsigned int line, string value) override {
        vector<string> cmdStrings = fileManager->readBuffer();
        vector<Command> eraseCmds;
        for (auto cmdString : cmdStrings) {
            if (cmdString == writeCommand(line, value))
                return;
            Command cmd = cmdFormat.parseCommand(cmdString);
            if (cmd.type == 'W' && cmd.lba == line) {
                fileManager->removeBuffer(writeCommand(cmd.lba, cmd.value));
            }
            else if (cmd.type == 'E') {
                eraseCmds.push_back(cmd);
            }
        }
        if (cmdStrings.size() >= MAX_COMMAND_NUM_IN_BUFFER)
            flush();
        fileManager->writeBuffer(writeCommand(line, value));
        updateErase(eraseCmds);
    }

    void read(unsigned int line) override {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Command cmd = cmdFormat.parseCommand(cmdString);

            if (cmd.type == 'W' && cmd.lba == line) {
                fileManager->writeResult(cmd.value);
                return;
            }
        }
        fileManager->readNand(line);
    }

    static bool compareByLba(const Command& a, const Command& b) {
        return a.lba < b.lba;
    }

    void erase(unsigned int line, unsigned int size) override {
        size = min(MAX_ERASE_SIZE, size);
        vector<string> cmdStrings = fileManager->readBuffer();
        vector<Command> eraseCmds;
        for (auto cmdString : cmdStrings) {
            Command cmd = cmdFormat.parseCommand(cmdString);
            if (cmd.type == 'W') {
                if (cmd.lba >= line && cmd.lba < line + size) {
                    fileManager->removeBuffer(writeCommand(cmd.lba, cmd.value));
                }
            }
            else if (cmd.type == 'E') {
                eraseCmds.push_back(cmd);
            }
        }
        if (fileManager->readBuffer().size() >= MAX_COMMAND_NUM_IN_BUFFER)
            flush();

        fileManager->writeBuffer(eraseCommand(line, size));

        eraseCmds.push_back(cmdFormat.parseCommand(eraseCommand(line, size)));
        mergeConsecutiveErases(eraseCmds);
    }

    void mergeConsecutiveErases(std::vector<Command>& eraseCmds) {
        std::sort(eraseCmds.begin(), eraseCmds.end(), compareByLba);
        int startLine = 0, endLine = 0, eraseSize = 0;
        bool consecutive = false;
        for (int i = 0; i < eraseCmds.size(); i++) {
            if (consecutive == false) {
                startLine = eraseCmds[i].lba;
                eraseSize = eraseCmds[i].size;
                endLine = eraseCmds[i].lba + eraseSize;
                consecutive = true;
                continue;
            }
            if (eraseCmds[i].lba == endLine) {
                fileManager->removeBuffer(eraseCommand(startLine, eraseSize));
                fileManager->removeBuffer(eraseCommand(eraseCmds[i].lba, eraseCmds[i].size));
                int newSize = eraseCmds[i].size + eraseSize;
                fileManager->writeBuffer(eraseCommand(startLine, newSize));
                eraseSize = newSize;
                endLine = startLine + eraseSize;
                consecutive = true;
            }
            else {
                consecutive = false;
            }
        }
    }

    void updateErase(std::vector<Command>& eraseCmds) {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Command cmd = cmdFormat.parseCommand(cmdString);
            if (cmd.type == 'W') {
                for (auto eraseCmd : eraseCmds) {
                    if (cmd.lba >= eraseCmd.lba
                        && cmd.lba < eraseCmd.lba + eraseCmd.size) {
                        fileManager->removeBuffer(eraseCommand(eraseCmd.lba, eraseCmd.size));
                        int formerSize = cmd.lba - eraseCmd.lba;
                        if(formerSize > 0) 
                            fileManager->writeBuffer(eraseCommand(eraseCmd.lba, formerSize));
                        int latterStart = cmd.lba + 1;
                        int latterSize = eraseCmd.lba + eraseCmd.size - latterStart;
                        if(latterSize > 0)
                            fileManager->writeBuffer(eraseCommand(latterStart, latterSize));
                    }
                }
            }
        }
    }



    void flush() {
        vector<string> cmdStrings = fileManager->readBuffer();
        for (auto cmdString : cmdStrings) {
            Command cmd = cmdFormat.parseCommand(cmdString);
            switch (cmd.type) {
            case 'R':
                fileManager->readNand(cmd.lba);
                break;
            case 'W':
                fileManager->writeNand(cmd.lba, cmd.value);
                break;
            case 'E':
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
    FileManager* fileManager;
    CommandFormat cmdFormat;

private:
    const string DEFAULT_VALUE = "0x00000000";
    const unsigned int MAX_COMMAND_NUM_IN_BUFFER = 10;
    const unsigned int MAX_ERASE_SIZE = 10;
};