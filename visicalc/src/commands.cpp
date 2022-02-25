#include "commands.h"
#include "input.h"

void commands::command()
{
	std::string exp;
	if (getinput(exp));
	if (exp.substr(0, 4) == "GOTO") {
		go_to(exp.substr(5));
	}
	else if (exp.substr(0, 4) == "SAVE") {
		Table::save();
	}
	else if (exp.substr(0, 4) == "LOAD") {
		Table::load();
	}
}

void commands::go_to(std::string args)
{
	Table::cursor = getcoords(args.c_str());
	Table::reset_view();
}
