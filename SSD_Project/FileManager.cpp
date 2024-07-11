#include "FileManager.h"

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
    ofstream resultFile = getResultFile();

    if (nandFile.is_open() == false || resultFile.is_open() == false)
        return;

    resultFile << getData(nandFile, line) << endl;

    nandFile.close();
    resultFile.close();
}

vector<string> FileManager::readBuffer() {
    vector<string> readLines;
    ifstream cmdBuffreRead(COMMAND_BUFFER_NAME);
    if (cmdBuffreRead.is_open())
        getAllData(cmdBuffreRead, readLines);
    return readLines;
}

void FileManager::writeBuffer(string command) {
    vector<string> commands = readBuffer();
    if (commands.size() >= 10) {
        flush(commands);
        //commands.clear();
    }
    commands.push_back(command);
    
    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, commands);
    cmdBufferWrite.close();
}

cmd_t FileManager::makeCmd(string &cmdstr) {
    cmd_t command = { '\0', 0, 0, "" };

    size_t pos1 = cmdstr.find(' ');
    size_t pos2 = cmdstr.find(' ', pos1 + 1);

    // command
    if (pos1 != std::string::npos) {
        command.cmd = cmdstr[0];
    }
    else {
        throw std::invalid_argument("Invalid command format");
    }

    if (cmdstr[0] == 'W') {
        // line
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            std::string line_str = cmdstr.substr(pos1 + 1, pos2 - pos1 - 1);
            try {
                command.index1 = std::stoi(line_str);
            }
            catch (const std::invalid_argument&) {
                throw std::invalid_argument("Invalid line number");
            }
        }
        else {
            throw std::invalid_argument("Invalid command format");
        }

        // data
        if (pos2 != std::string::npos) {
            command.data = cmdstr.substr(pos2 + 1);
        }
        else {
            throw std::invalid_argument("Invalid command format");
        }
    }
    else if (cmdstr[0] == 'E') {
        std::string rest = cmdstr.substr(2);

        // index1
        size_t first_space = rest.find(' ');
        if (first_space == std::string::npos) {
            throw std::invalid_argument("Invalid command format");
        }
        command.index1 = std::stoi(rest.substr(0, first_space));

        // index2
        size_t second_space = rest.find(' ', first_space);
        if (second_space == std::string::npos) {
            throw std::invalid_argument("Invalid command format");
        }
        command.index2 = std::stoi(rest.substr(first_space + 1, second_space - first_space - 1));

        // data
        command.data = rest.substr(second_space + 1);
    }
    return command;
}

void FileManager::flush(vector<string>& commands) {
    for (std::string cmdstr : commands) {
        cmd_t cmd = makeCmd(cmdstr);
        if (cmd.cmd == 'W') {
            writeNand(cmd.index1, cmd.data);
        }
        else if (cmd.cmd == 'E') {
            for (int i = cmd.index1; i < cmd.index2; i++) {
                writeNand(i, DEFAULT_WRITE_VALUE);
            }
        }
    }
    commands.clear();
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