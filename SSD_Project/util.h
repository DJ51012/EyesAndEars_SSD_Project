#pragma once
#include<iostream>

using namespace std;

struct Command {
    char type;  // 'R', 'W', 'E', 'F'
    int lba;
    int size;
    string value;
};

class CommandFormat {
public:
    Command parseCommand(string commandString) {
        vector<string> splitCmd;
        std::string temp;
        
        for (char ch : commandString) {
            if (ch == ' ') {
                if (!temp.empty()) {
                    splitCmd.push_back(temp);
                    temp.clear();
                }
            }
            else {
                temp += ch;
            }
        }

        if (!temp.empty()) {
            splitCmd.push_back(temp);
        }

        Command command;
        command.type = splitCmd[0][0];
        command.lba = stoi(splitCmd[1]);
        if (command.type == 'W')
            command.value = splitCmd[2];
        else if (command.type == 'E')
            command.size = stoi(splitCmd[2]);
        return command;
    }
};