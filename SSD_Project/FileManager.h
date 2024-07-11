#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
struct cmd_type {
    char cmd;
    int index1;
    int index2;
    string data;
};
typedef struct cmd_type cmd_t;

class FileManager {
public:
#ifdef __TEST__
    FileManager();
#endif
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }
    virtual void writeNand(unsigned int line, string value);
    virtual void readNand(unsigned int line);

    virtual void writeBuffer(string command);
    cmd_t makeCmd(string& cmdstr);
    virtual vector<string> readBuffer();
    virtual void flush(vector<string> &commands);
private:
#ifndef __TEST__
    FileManager();
#endif
    FileManager& operator=(const FileManager& otherInstance) = delete;
    FileManager(const FileManager& otherInstance) = delete;
    void createFile(string fileName);
    ifstream getNandFile();
    ofstream getResultFile();
    ifstream getCommandBuffer();
    bool checkValidLineNum(int line);
    bool getAllData(ifstream& readFile, vector<string>& readLines);
    void setAllData(ofstream& writeFile, vector<string>& readLines);
    string getData(ifstream& nandFile, unsigned int line);

    const int MAX_LINE_NUM = 99;
    const int MIN_LINE_NUM = 0;
    const string NAND_FILE_NAME = "nand.txt";
    const string RESULT_FILE_NAME = "result.txt";
    const string COMMAND_BUFFER_NAME = "buffer.txt";
    const string DEFAULT_WRITE_VALUE = "0x00000000";
};