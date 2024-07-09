#include "FileManager.h"

FileManager::FileManager() {

}

void FileManager::writeNand(unsigned int line, string value) {
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
    nandFile.close();

    ofstream nandFileWrite(NAND_FILE_NAME);

    for (const auto& ln : readLines) {
        nandFileWrite << ln << endl;
    }
    nandFileWrite.close();
}

void FileManager::readNand(unsigned int line) {
    fstream nandFile = getNandFile();
    ofstream resultFile = getResultFile();

    string readLine;
    int lineNumber = 0;
    string value;

    while (getline(nandFile, readLine)) {
        if (lineNumber == line) {
            resultFile << readLine << endl;
            break;
        }
        lineNumber++;
    }

    nandFile.close();
    resultFile.close();
}

void FileManager::createFile() {
    ofstream nandfile(NAND_FILE_NAME);
    for (int idx = 0; idx <= MAX_LINE_NUM; idx++) {
        nandfile << DEFAULT_WRITE_VALUE << endl;
    }
    nandfile.close();
}

fstream FileManager::getNandFile() {
    ifstream nandfileCheck(NAND_FILE_NAME);
    if (!nandfileCheck.is_open()) {
        createFile();
    }
    fstream nandfile(NAND_FILE_NAME);
    return nandfile;
}

ofstream FileManager::getResultFile() {
    ofstream resultFile(RESULT_FILE_NAME);
    return resultFile;
}

void FileManager::validLineNumCheck(int line) {
    if (line < MIN_LINE_NUM || line > MAX_LINE_NUM) {
        cerr << "Invalid line number" << endl;
    }
}

