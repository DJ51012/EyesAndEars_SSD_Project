#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../SSD_Project/FileManager.cpp"

using namespace testing;

class FileMangerFixture : public testing::Test {
public:
	FileManager fileManager;
	const string DATA = "0x1004";
	const string DATA2 = "0x1005";
};

TEST_F(FileMangerFixture, ReadTestInitLba0) {
	string data = fileManager.readNand(0);
	EXPECT_THAT(data, StrEq(fileManager.CLEAN_DATA));
}

TEST_F(FileMangerFixture, ReadTestInitLba99) {
	string data = fileManager.readNand(99);
	EXPECT_THAT(data, StrEq(fileManager.CLEAN_DATA));
}

TEST_F(FileMangerFixture, WriteReadTestLba0) {
	fileManager.writeNand(0, DATA);
	string data = fileManager.readNand(0);
	EXPECT_THAT(data, StrEq(DATA));
}

TEST_F(FileMangerFixture, WriteReadTestLba99) {
	fileManager.writeNand(99, DATA);
	string data = fileManager.readNand(99);
	EXPECT_THAT(data, StrEq(DATA));
}

TEST_F(FileMangerFixture, WriteReadTestLba0_1) {
	fileManager.writeNand(10, DATA);
	fileManager.writeNand(10, DATA2);
	string data = fileManager.readNand(10);
	EXPECT_THAT(data, StrEq(DATA2));
}

TEST_F(FileMangerFixture, WriteReadTestCleanLba) {
	fileManager.writeNand(10, DATA);
	string data = fileManager.readNand(20);
	EXPECT_THAT(data, StrEq(fileManager.CLEAN_DATA));
}
