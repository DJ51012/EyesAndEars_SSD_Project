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
        FileManager& fileManager = FileManager::getInstance();
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

    void flush() {}


    FileManager* fileManager;
};