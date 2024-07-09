#pragma once

#include <fstream>
#include <string>

using namespace std;

class FileManager {
public:
	const string CLEAN_DATA = "0x00000000";

	FileManager();
	string readNand(int lba);
	void writeNand(int lba, string data);

private:
	const string FILE_NAME = "nand.txt";
	const int MAX_LBA = 100;
	string nand[100];
};