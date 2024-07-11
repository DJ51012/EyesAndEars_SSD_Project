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
        
        Command command;

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

        command.type = splitCmd[0][0];
        command.lba = stoi(splitCmd[1]);
        command.value = splitCmd[2];
        return command;
    }


};