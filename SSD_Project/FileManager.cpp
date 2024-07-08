#include "FileManager.h"

unsigned int FileManager::readNand(int lba) {
	return nand[lba];
}

void FileManager::writeNand(int lba, unsigned int data) {
	nand[lba] = data;
}