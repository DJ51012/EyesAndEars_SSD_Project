#pragma once

#include <fstream>


class FileManager {
public:
	FileManager() : nand{} {}
	unsigned int readNand(int lba);
	void writeNand(int lba, unsigned int data);
private:
	unsigned int nand[100];
};