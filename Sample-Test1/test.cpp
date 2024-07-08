#include "pch.h"

#include "../SSD_Project/CommandManager.h"

using namespace testing;


// Command Manager

class CommandManagerFixture : public testing::Test
{
public:
	CommandManager cm;
};

TEST_F(CommandManagerFixture, No_Command_Code) 
{
	int argc = 1;
	char* argv[] = { "ssd"};

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;
	
	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Read)
{
	int argc = 1;
	char* argv[] = { "ssd", "R"};

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandManagerFixture, No_Address_Write)
{
	int argc = 1;
	char* argv[] = { "ssd", "W"};

	bool actual = cm.IsValidCommand(argc, argv);
	bool expected = false;

	EXPECT_EQ(expected, actual);
}