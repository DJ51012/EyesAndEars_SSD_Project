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

    PRINT_LOG("Write Nand Success");
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

    PRINT_LOG("Read Nand Success");
}

vector<string> FileManager::readBuffer() {
    vector<string> readLines;
    ifstream cmdBuffreRead(COMMAND_BUFFER_NAME);
    if (cmdBuffreRead.is_open())
        getAllData(cmdBuffreRead, readLines);

    PRINT_LOG("Read Buffer Success");
    return readLines;
}

void FileManager::writeBuffer(string command) {
    vector<string> commands = readBuffer();
    if (commands.size() >= MAX_COMMAND_NUM_IN_BUFFER) {
        flushBuffer();
        commands.clear();
    }
    commands.push_back(command);
    
    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, commands);
    cmdBufferWrite.close();

    PRINT_LOG("Write Buffer Success");
}

void FileManager::removeBuffer(string command) {
    vector<string> cmdStrings = readBuffer();
    cmdStrings.erase(remove(cmdStrings.begin(), 
        cmdStrings.end(), command), cmdStrings.end());

    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, cmdStrings);
    cmdBufferWrite.close();
}

void FileManager::flushBuffer() {
    vector<string> initCommands;
    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, initCommands);
    cmdBufferWrite.close();
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

ifstream FileManager::getCommandBuffer() {
    ifstream cmdBufferForCheck(COMMAND_BUFFER_NAME);
    if (cmdBufferForCheck.is_open() == false) {
        createFile(COMMAND_BUFFER_NAME);
    }
    ifstream commandBuffer(COMMAND_BUFFER_NAME);
    return commandBuffer;
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