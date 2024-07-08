#include "FileManager.h"

FileManager::FileManager() {
	for (int i = 0; i < MAX_LBA; i++)
		nand[i] = CLEAN_DATA;
}

string FileManager::readNand(int lba) {
	return nand[lba];
}

void FileManager::writeNand(int lba, string data) {
	nand[lba] = data;
}