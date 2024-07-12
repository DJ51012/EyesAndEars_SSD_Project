#include "FileManager.h"
#include "../Logger/Logger.h"

FileManager::FileManager() {

}

void FileManager::writeNand(unsigned int line, string value) {
    vector<string> readLines;
    ifstream nandFileRead = getNandFile();
    if (checkValidLineNum(line) == false 
        || getAllData(nandFileRead, readLines) == false)
        return;
    readLines[line] = value;

    ofstream nandFileWrite(NAND_FILE_NAME);
    setAllData(nandFileWrite, readLines);
    
    nandFileRead.close();
    nandFileWrite.close();
}

void FileManager::readNand(unsigned int line) {
    ifstream nandFile = getNandFile();

    if (nandFile.is_open() == false)
        return;

    writeResult(getData(nandFile, line));
    nandFile.close();
}

void FileManager::writeResult(string value) {
    ofstream resultFile = getResultFile();
    resultFile << value << endl;
    resultFile.close();
}


void FileManager::createFile(string fileName) {
    ofstream writeFile(fileName);
    if (fileName == NAND_FILE_NAME) {
        for (int idx = 0; idx <= MAX_LINE_NUM; idx++) {
            writeFile << DEFAULT_WRITE_VALUE << endl;
        }
    }
    writeFile.close();
}

ifstream FileManager::getNandFile() {
    ifstream nandFileForCheck(NAND_FILE_NAME);
    if (nandFileForCheck.is_open() == false) {
        createFile(NAND_FILE_NAME);
    }
    ifstream nandFile(NAND_FILE_NAME);
    return nandFile;
}

ofstream FileManager::getResultFile() {
    ofstream resultFile(RESULT_FILE_NAME);
    return resultFile;
}


bool FileManager::checkValidLineNum(int line) {
    if (line < MIN_LINE_NUM || line > MAX_LINE_NUM) {
        cerr << "Invalid line number" << endl;
        return false;
    }
    return true;
}

bool FileManager::getAllData(ifstream& readFile, vector<string>& readLines)
{
    string strLine = "";

    while (getline(readFile, strLine)) {
        readLines.push_back(strLine);
    }
    return true;
}

void FileManager::setAllData(ofstream& writeFile, vector<string>& readLines)
{
    for (const auto& lineData : readLines) {
        writeFile << lineData << endl;
    }
}

string FileManager::getData(ifstream& nandFile, unsigned int line) {
    int lineNumber = 0;
    string strLine = "";
    while (getline(nandFile, strLine)) {
        if (lineNumber == line) {
            return strLine;
        }
        lineNumber++;
    }
    return strLine;
}