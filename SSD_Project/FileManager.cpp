#include "FileManager.h"
#include "../Logger/Logger.h"

FileManager::FileManager() {

}

void FileManager::writeNand(unsigned int line, string value) {
    PRINT("writeNand");
    //Logger::getInstance().print("writeNand", __FUNCTION__);
    vector<string> readLines;

    if (checkValidLineNum(line) == false || getAllData(readLines) == false)
        return;

    readLines[line] = value;

    setAllData(readLines);
}

void FileManager::readNand(unsigned int line) {
    PRINT("readNand");
    //Logger::getInstance().print("readNand", __FUNCTION__);
    fstream nandFile = getNandFile();
    ofstream resultFile = getResultFile();

    if (nandFile.is_open() == false || resultFile.is_open() == false)
        return;

    resultFile << getData(nandFile, line) << endl;

    nandFile.close();
    resultFile.close();
}

void FileManager::createFile() {
    PRINT("createFile");
    //Logger::getInstance().print("createFile", __FUNCTION__);
    ofstream nandfile(NAND_FILE_NAME);
    for (int idx = 0; idx <= MAX_LINE_NUM; idx++) {
        nandfile << DEFAULT_WRITE_VALUE << endl;
    }
    if(nandfile.is_open() == false)
        PRINT("createFile Failed!");
    nandfile.close();
}

fstream FileManager::getNandFile() {
    ifstream nandFileForCheck(NAND_FILE_NAME);
    if (nandFileForCheck.is_open() == false) {
        createFile();
    }
    fstream nandfile(NAND_FILE_NAME);
    return nandfile;
}

ofstream FileManager::getResultFile() {
    ofstream resultFile(RESULT_FILE_NAME);
    if (resultFile.is_open() == false)
        PRINT("resultFile Failed!");
    return resultFile;
}

bool FileManager::checkValidLineNum(int line) {
    if (line < MIN_LINE_NUM || line > MAX_LINE_NUM) {
        cerr << "Invalid line number" << endl;
        return false;
    }
    return true;
}

bool FileManager::getAllData(vector<string>& readLines)
{
    string strLine = "";
    fstream nandFile = getNandFile();
    if (nandFile.is_open() == false)
        return false;

    while (getline(nandFile, strLine)) {
        readLines.push_back(strLine);
    }
    nandFile.close();
    return true;
}

void FileManager::setAllData(vector<string>& readLines)
{
    ofstream nandFileWrite(NAND_FILE_NAME);

    for (const auto& lineData : readLines) {
        nandFileWrite << lineData << endl;
    }
    nandFileWrite.close();
}

string FileManager::getData(fstream& nandFile, unsigned int line) {
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