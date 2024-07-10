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
        while (fgets(command, sizeof(command), file) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }
            ts->setDriverRedirection(true);

            if (SCENARIO_FULLREADWRITECOMPARE == command) {
                fullReadWriteCompare(ts);
            }
            else if (SCENARIO_WRITE30ANDOVERWRITE == command) {
                Write30AndOwerWrite(ts);
            }
        }
        set_oringin_std_inout();
        fclose(file);
    }


    int fullReadWriteCompare(TestShell* ts) {
        LOG(command);
        LOG(" --- Run...");
        if (ts->run_shell("testapp1") == 0) {
            LOG("Pass\n");
            return 0;
        }
        else {
            LOG("FAIL!\n");
            set_oringin_std_inout();
            return -1;
        }
    }

    int Write30AndOwerWrite(TestShell* ts) {
        LOG(command);
        LOG(" --- Run...");
        if (ts->run_shell("testapp2") == 0) {
            LOG("Pass\n");
            return 0;
        }
        else {
            LOG("FAIL!\n");
            set_oringin_std_inout();
            return -1;
        }
        return 0;
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
    char command[1000];
};
