#pragma once
#include "SsdDriver.h"
#include "FileManager.h"

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
        fileManager->writeBuffer(writeCommand(line, value));
    }

    vector<string> splitCommand(string cmd) {
        istringstream iss(cmd);
        string token;
        vector<string> tokens;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void read(unsigned int line) override {
        commands = fileManager->readBuffer();
        for (auto cmd : commands) {
            vector<string> splitCmd = splitCommand(cmd); 
            char rw = splitCmd[0][0];
            unsigned int lineNum = stoi(splitCmd[1]);
            string value = splitCmd[2];
            if (rw == 'W' && lineNum == line) {
                fileManager->writeResult(value);
                return;
            }
        }
        fileManager->readNand(line);
    }

    void flush() {
        commands = fileManager->readBuffer();
        //for (const auto &cmd : commands) {
        //    //parse command
        //    // if read
        //    read(line);
        //    //if write
        //    write(line)
        //}
    }



    void readCmdBuffer() {
        commands = fileManager->readBuffer();
    }

    vector<string> commands;
    FileManager* fileManager;
};