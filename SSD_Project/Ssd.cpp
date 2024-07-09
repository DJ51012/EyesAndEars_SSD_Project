#include "SsdDriver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Ssd : public SsdDriver {
public:
    ofstream createFile() {
        std::ofstream nandfile(NAND_FILE_NAME);
        for (int idx = 0; idx < 100; idx++) {
            nandfile << DEFAULT_WRITE_VALUE << std::endl;
        }
        nandfile.close();
        return nandfile;
    }

    fstream getNandFile() {
        std::ifstream nandfileCheck(NAND_FILE_NAME);
        if (!nandfileCheck.is_open()) {
            createFile();
        }
        std::fstream nandfile(NAND_FILE_NAME);
        return nandfile;
    }

    ofstream getResultFile() {
        std::ofstream resultFile(RESULT_FILE_NAME);
        return resultFile;
    }
    
    void validLineNumCheck(int line) {
        if (line < MIN_LINE_NUM || line > MAX_LINE_NUM) {
            cerr << "Invalid line number" << endl;
        }
    }

    void write(unsigned int line, string value) override {
        std::vector<std::string> readLines;
        std::string readLine;

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
            nandFile  << ln << std::endl;
        }
        nandFile.close();
    }

    string read(unsigned int line) {
        fstream nandFile = getNandFile();
        ofstream resultFile = getResultFile();

        string readLine;
        int lineNumber = 0;
        string value;

        while (std::getline(nandFile, readLine)) {
            if (lineNumber == line) {
                istringstream iss(readLine);
                if (iss >> value) {
                    resultFile << value << std::endl;
                }
                else {
                    std::cerr << "The value in line " << line << " is not a valid string" << std::endl;
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