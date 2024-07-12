#include "BufferManager.h"
#include "../Logger/Logger.h"

BufferManager::BufferManager() {

}

vector<string> BufferManager::readBuffer() {
    vector<string> readLines;
    ifstream cmdBuffreRead(COMMAND_BUFFER_NAME);
    if (cmdBuffreRead.is_open())
        getAllData(cmdBuffreRead, readLines);
    return readLines;
}

void BufferManager::writeBuffer(string command) {
    vector<string> commands = readBuffer();
    if (commands.size() >= MAX_COMMAND_NUM_IN_BUFFER) {
        flushBuffer();
        commands.clear();
    }
    commands.push_back(command);

    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, commands);
    cmdBufferWrite.close();

    PRINT_LOG(("Write Buffer succeeded / Command: " + command));
}

void BufferManager::removeBuffer(string command) {
    vector<string> cmdStrings = readBuffer();
    cmdStrings.erase(remove(cmdStrings.begin(),
        cmdStrings.end(), command), cmdStrings.end());

    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, cmdStrings);
    cmdBufferWrite.close();
}

void BufferManager::flushBuffer() {
    vector<string> initCommands;
    ofstream cmdBufferWrite(COMMAND_BUFFER_NAME);
    setAllData(cmdBufferWrite, initCommands);
    cmdBufferWrite.close();
}

ifstream BufferManager::getCommandBuffer() {
    ifstream cmdBufferForCheck(COMMAND_BUFFER_NAME);
    if (cmdBufferForCheck.is_open() == false) {
        createFile(COMMAND_BUFFER_NAME);
    }
    ifstream commandBuffer(COMMAND_BUFFER_NAME);
    return commandBuffer;
}

void BufferManager::mergeConsecutiveErases(vector<string>& erasemdStrings) {
    vector<Command> eraseCmds;
    for (auto cmdString : erasemdStrings) {
        eraseCmds.push_back(cmdFormat.parseCommand(cmdString));
    }
    std::sort(eraseCmds.begin(), eraseCmds.end(), cmdFormat.compareByLba);
    int startLine = 0, endLine = 0, eraseSize = 0;
    bool consecutive = false;
    for (int i = 0; i < eraseCmds.size(); i++) {
        if (consecutive == false) {
            startLine = eraseCmds[i].lba;
            eraseSize = eraseCmds[i].size;
            endLine = eraseCmds[i].lba + eraseSize;
            consecutive = true;
            continue;
        }
        if (eraseCmds[i].lba == endLine) {
            removeBuffer(eraseCommand(startLine, eraseSize));
            removeBuffer(eraseCommand(eraseCmds[i].lba, eraseCmds[i].size));
            int newSize = eraseCmds[i].size + eraseSize;
            writeBuffer(eraseCommand(startLine, newSize));
            eraseSize = newSize;
            endLine = startLine + eraseSize;
            consecutive = true;
        }
        else {
            consecutive = false;
        }
    }
}

void BufferManager::updateErase(std::vector<string>& eraseCmdStrings) {
    vector<Command> eraseCmds;
    for (auto cmdString : eraseCmdStrings) {
        eraseCmds.push_back(cmdFormat.parseCommand(cmdString));
    }

    vector<string> cmdStrings = readBuffer();
    for (auto cmdString : cmdStrings) {
        Command cmd = cmdFormat.parseCommand(cmdString);
        if (cmd.type == 'W') {
            for (auto eraseCmd : eraseCmds) {
                if (cmd.lba >= eraseCmd.lba
                    && cmd.lba < eraseCmd.lba + eraseCmd.size) {
                    removeBuffer(eraseCommand(eraseCmd.lba, eraseCmd.size));
                    int formerSize = cmd.lba - eraseCmd.lba;
                    if (formerSize > 0)
                        writeBuffer(eraseCommand(eraseCmd.lba, formerSize));
                    int latterStart = cmd.lba + 1;
                    int latterSize = eraseCmd.lba + eraseCmd.size - latterStart;
                    if (latterSize > 0)
                        writeBuffer(eraseCommand(latterStart, latterSize));
                }
            }
        }
    }
}
