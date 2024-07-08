//#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../SSD_Project/CommandManager.h"

using namespace testing;


// Command Manager
class SsdDriverMock : public SsdDriver
{
public:
	MOCK_METHOD(void, write, (unsigned int lba_index, string value), (override));
	MOCK_METHOD(string, read, (unsigned int lba_index), (override));
};

class CommandManagerFixture : public testing::Test
{
public:
	CommandManager cm;
	SsdDriverMock mock;
};

TEST_F(CommandManagerFixture, No_Command_Code)
{
	int argc = 1;
	char* argv[] = { "ssd" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Read)
{
	int argc = 1;
	char* argv[] = { "ssd", "R" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Write)
{
	int argc = 1;
	char* argv[] = { "ssd", "W" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Valid_Write)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "0", "0xABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = true;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Valid_Read)
{
	int argc = 3;
	char* argv[] = { "ssd", "R", "99" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = true;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_CommandCode000)
{
	int argc = 4;
	char* argv[] = { "ssd", "4", "0", "ABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}


TEST_F(CommandManagerFixture, Invalid_CommandCode001)
{
	int argc = 3;
	char* argv[] = { "ssd", "q", "0" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}


TEST_F(CommandManagerFixture, Invalid_Address000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "a", "ABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Address001)
{
	int argc = 4;
	char* argv[] = { "ssd", "R", "100", "ABCD1234" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Data000)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "50", "0xqw0-1231zlomas8=" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Invalid_Data001)
{
	int argc = 4;
	char* argv[] = { "ssd", "W", "40", "0x!231+asd" };

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}


TEST_F(CommandManagerFixture, Execute_Write)
{
	// Arrange
	int argc = 4;
	char* argv[] = { "ssd", "W", "0", "0xABCD1234" };
	bool expected = true;
	EXPECT_CALL(mock, write(0, "0xABCD1234"));

	// Act
	bool actual = cm.IsValidCommand(argc, argv);
	cm.executeSSDCommand(&mock);

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Execute_Read)
{
	// Arrange
	int argc = 3;
	char* argv[] = { "ssd", "R", "99" };
	bool expected = true;
	EXPECT_CALL(mock, read(99));

	// Act
	bool actual = cm.IsValidCommand(argc, argv);
	cm.executeSSDCommand(&mock);

	// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, Execute_Nothing)
{
	// Arrange
	EXPECT_CALL(mock, read)
		.Times(0);
	EXPECT_CALL(mock, write)
		.Times(0);

	// Act
	cm.executeSSDCommand(&mock);
}