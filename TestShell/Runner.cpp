#pragma once
#include<iostream>
#include "testshell.cpp"

using namespace std;

class Runner {
public:
    void start_runner(TestShell* ts, char* filename) {
        FILE* file;
        errno_t err = fopen_s(&file, filename, "r");
        if (err != 0) {
            perror("파일을 열 수 없습니다");
            return;
        }
        backup_std_inout();
        set_user_std_inout();
        char command[100];
        while (fgets(command, sizeof(command), file) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }
            ts->setDriverRedirection(true);
            if(run_scenario(ts, command) != 0){
                break;
            }
        }
        set_oringin_std_inout();
        fclose(file);
    }

    int run_scenario(TestShell* ts, char* cmd) {
        LOG(cmd);
        LOG(" --- Run...");
        if (ts->run_shell(cmd) == 0) {
            LOG("Pass\n");
            return 0;
        }
        else {
            LOG("FAIL!\n");
            return -1;
        }
    }

private:
    void LOG(string str) {
        set_oringin_std_inout();
        std::cout << str;
        set_user_std_inout();
    }

    void backup_std_inout() {
        original_cin_buf = std::cin.rdbuf();
        original_cout_buf = std::cout.rdbuf();
    }

    void set_oringin_std_inout() {
        std::cin.rdbuf(original_cin_buf);
        std::cout.rdbuf(original_cout_buf);
    }

    void set_user_std_inout() {
        std::cin.rdbuf(std_input.rdbuf());
        std::cout.rdbuf(std_output.rdbuf());
    }

    std::streambuf* original_cin_buf;
    std::streambuf* original_cout_buf;
    std::istringstream std_input;
    std::ostringstream std_output;
    const string SCENARIO_FULLREADWRITECOMPARE = "FullReadWriteCompare";
    const string SCENARIO_WRITE30ANDOVERWRITE = "Write30AndOwerWrite";
};
