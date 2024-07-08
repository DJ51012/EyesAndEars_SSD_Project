#include "FileManager.h"

string FileManager::readNand(int lba) {
	return nand[lba];
}

void FileManager::writeNand(int lba, string data) {
	nand[lba] = data;
}