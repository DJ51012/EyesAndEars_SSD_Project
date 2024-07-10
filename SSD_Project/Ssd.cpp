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

    void write(unsigned int line, string value) override {
        fileManager->writeNand(line, value);
    }

    void read(unsigned int line) override {
        fileManager->readNand(line);
    }

    void erase(unsigned int line, unsigned int size) override {
        for (unsigned int offset = 0; offset < size; offset++) {
            fileManager->writeNand((line + offset), DEFAULT_VALUE);
        }
    }

    FileManager* fileManager;

private:
    const string DEFAULT_VALUE = "0x00000000";
};