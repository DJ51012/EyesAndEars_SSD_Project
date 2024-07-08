#include "pch.h"

#include "../SSD_Project/FileManager.cpp"

class FileMangerFixture : public testing::Test {
public:
	FileManager fileManager;
	const int DATA = 1004;
};

TEST_F(FileMangerFixture, ReadTestInitLba0) {
	unsigned int data = fileManager.readNand(0);
	EXPECT_EQ(data, 0);
}

TEST_F(FileMangerFixture, ReadTestInitLba99) {
	unsigned int data = fileManager.readNand(99);
	EXPECT_EQ(data, 0);
}

TEST_F(FileMangerFixture, WriteReadTestLba0) {
	fileManager.writeNand(0, DATA);
	unsigned int data = fileManager.readNand(0);
	EXPECT_EQ(data, DATA);
}

TEST_F(FileMangerFixture, WriteReadTestLba99) {
	fileManager.writeNand(99, DATA);
	unsigned int data = fileManager.readNand(99);
	EXPECT_EQ(data, DATA);
}

TEST_F(FileMangerFixture, WriteReadTestLba0_1) {
	fileManager.writeNand(10, DATA);
	fileManager.writeNand(10, DATA + 1);
	unsigned int data = fileManager.readNand(10);
	EXPECT_EQ(data, DATA + 1);
}

TEST_F(FileMangerFixture, WriteReadTestCleanLba) {
	fileManager.writeNand(10, DATA);
	unsigned int data = fileManager.readNand(20);
	EXPECT_EQ(data, 0);
}