#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class FileManager {
public:
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }
    void writeNand(unsigned int line, string value);
    void readNand(unsigned int line);

private:
    FileManager();
    FileManager& operator=(const FileManager& otherInstance) = delete;
    FileManager(const FileManager& otherInstance) = delete;
    void cleanNand();
    void createFile();
    fstream getNandFile();
    ofstream getResultFile();
    bool checkValidLineNum(int line);
    bool getAllData(vector<string>& readLines);
    void setAllData(vector<string>& readLines);
    string getData(fstream& nandFile, unsigned int line);

    const int MAX_LINE_NUM = 99;
    const int MIN_LINE_NUM = 0;
    const string NAND_FILE_NAME = "nand.txt";
    const string RESULT_FILE_NAME = "result.txt";
    const string DEFAULT_WRITE_VALUE = "0x00000000";
};