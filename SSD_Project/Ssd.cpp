#include "SsdDriver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Ssd : public SsdDriver {
public:
    ofstream createFile() {
        ofstream nandfile(NAND_FILE_NAME);
        for (int idx = 0; idx < 100; idx++) {
            nandfile << DEFAULT_WRITE_VALUE << endl;
        }
        nandfile.close();
        return nandfile;
    }

    fstream getNandFile() {
        ifstream nandfileCheck(NAND_FILE_NAME);
        if (!nandfileCheck.is_open()) {
            createFile();
        }
        fstream nandfile(NAND_FILE_NAME);
        return nandfile;
    }

    ofstream getResultFile() {
        ofstream resultFile(RESULT_FILE_NAME);
        return resultFile;
    }
    
    void validLineNumCheck(int line) {
        if (line < MIN_LINE_NUM || line > MAX_LINE_NUM) {
            cerr << "Invalid line number" << endl;
        }
    }

    void write(unsigned int line, string value) override {
        vector<string> readLines;
        string readLine;

        fstream nandFile = getNandFile();
        if (nandFile.is_open()) {
            while (getline(nandFile, readLine)) {
                readLines.push_back(readLine);
            }
        }
        validLineNumCheck(line);
        
        readLines[line] = value;

        nandFile.clear();
        nandFile.seekp(0, ios::beg);
        for (const auto& ln : readLines) {
            nandFile  << ln << endl;
        }
        nandFile.close();
    }

    string read(unsigned int line) {
        fstream nandFile = getNandFile();
        ofstream resultFile = getResultFile();

        string readLine;
        int lineNumber = 0;
        string value;

        while (getline(nandFile, readLine)) {
            if (lineNumber == line) {
                istringstream iss(readLine);
                if (iss >> value) {
                    resultFile << value << endl;
                }
                else {
                    cerr << "The value in line " << line << " is not a valid string" << endl;
                }
                break;
            }
            lineNumber++;
        }

        nandFile.close();
        resultFile.close();
        return value;
    }

    const int MAX_LINE_NUM = 99;
    const int MIN_LINE_NUM = 0;
    const string NAND_FILE_NAME = "nand.txt";
    const string RESULT_FILE_NAME = "result.txt";
    const string DEFAULT_WRITE_VALUE = "0x00000000";
};