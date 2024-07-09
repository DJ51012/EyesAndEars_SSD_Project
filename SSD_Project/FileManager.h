#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class FileManager {
public:
//#ifdef __TEST__ 
    FileManager();
//#endif
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }
    void writeNand(unsigned int line, string value);
    void readNand(unsigned int line);

private:
//#ifndef __TEST__
    //FileManager();
//#endif 

    FileManager& operator=(const FileManager& otherInstance) = delete;
    FileManager(const FileManager& otherInstance) = delete;
    void createFile();
    fstream getNandFile();
    ofstream getResultFile();
    void validLineNumCheck(int line);

    const int MAX_LINE_NUM = 99;
    const int MIN_LINE_NUM = 0;
    const string NAND_FILE_NAME = "nand.txt";
    const string RESULT_FILE_NAME = "result.txt";
    const string DEFAULT_WRITE_VALUE = "0x00000000";
};