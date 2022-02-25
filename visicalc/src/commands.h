#pragma once
#include <string>
#include "table.h"

class commands
{
public:
	static void command();
	static void menu();
	static void go_to(std::string args);
};