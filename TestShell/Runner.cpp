#pragma once
#include<iostream>
#include "testshell.cpp"

using namespace std;

class Runner {
public:
	void start_runner(TestShell* ts, char* filename) {
        printf("read : %s\n", filename);
		FILE* file;
        errno_t err = fopen_s(&file, filename, "r");
        if (err != 0) {
            perror("파일을 열 수 없습니다");
            return ;
        }

        while (fgets(command, sizeof(command), file) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }

            if (SCENARIO_1 == command) {
            }
            else if (SCENARIO_2 == command) {
            }
        }
        fclose(file);
	}
private:
    const string SCENARIO_1 = "TestApp1";
    const string SCENARIO_2 = "TestApp2";
    char command[1000];
};
