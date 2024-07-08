#pragma once

#include <fstream>
#include <string>

using namespace std;

class FileManager {
public:
	const string CLEAN_DATA = "0x00000000";

	FileManager(){
		for (int i = 0; i < 100; i++)
			nand[i] = CLEAN_DATA;
	}
	string readNand(int lba);
	void writeNand(int lba, string data);
private:
	string nand[100];
};