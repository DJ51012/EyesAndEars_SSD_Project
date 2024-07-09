#include "SsdDriver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class MySsdDriver : public SsdDriver {
public:
    ofstream createFile() {
        std::ofstream nandfile("nand.txt");
        for (int idx = 0; idx < 99; idx++) {
            nandfile << "0" << std::endl;
        }
        nandfile.close();
        return nandfile;
    }

    fstream getNandFile() {
        std::ifstream nandfileCheck("nand.txt");
        if (!nandfileCheck.is_open()) {
            createFile();
        }
        std::fstream nandfile("nand.txt");
        return nandfile;
    }

    ofstream getResultFile() {
        std::ofstream resultFile("result.txt");
        return resultFile;
    }

    void write(unsigned int n, string value) override {
        std::vector<std::string> lines;
        std::string line;

        fstream nandFile = getNandFile();
        if (nandFile.is_open()) {
            while (std::getline(nandFile, line)) {
                lines.push_back(line);
            }
        }

        if (n <= 0 || n >= 100) {
            std::cerr << "Invalid line number" << std::endl;
            return;
        }

        lines[n-1] = value;

        nandFile.clear();
        nandFile.seekp(0, std::ios::beg);
        for (const auto& ln : lines) {
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
            lineNumber++;
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
        }

        nandFile.close();
        resultFile.close();
        return value;
    }
};