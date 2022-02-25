#include "input.h"

int getinput(std::string & str) {
	Table::set_cursor_visibility(true); //show console cursor
	printf("\033[3;0H%s", str.c_str()); //move to 3rd line, print str

	std::string def = str;
	char ch = _getch();
	while (ch != 13) {
		if (ch == 27) { //esc
			printf("\033[2K");
			Table::set_cursor_visibility(false);
			str = def;
			return 0;
		};
		if (ch == 8) str = str.substr(0, str.length() - 1);
		else str += ch;
		printf("\033[2K\033[0G%s", str.c_str());
		ch = _getch();
	}

	printf("\033[2K"); //clear the line
	Table::set_cursor_visibility(false); //hide console cursor again

	return 1;
}

int getinput(entry* ent) {
	Table::set_cursor_visibility(true); //show console cursor
	printf("\033[3;0H%s", str.c_str()); //move to 3rd line, print str

	std::string def = str;
	char ch = _getch();
	while (ch != 13) {
		if (ch == 27) { //esc
			printf("\033[2K");
			Table::set_cursor_visibility(false);
			str = def;
			return 0;
		};
		if (ch == 8) str = str.substr(0, str.length() - 1);
		else str += ch;
		printf("\033[2K\033[0G%s", str.c_str());
		ch = _getch();
	}

	printf("\033[2K"); //clear the line
	Table::set_cursor_visibility(false); //hide console cursor again

	return 1;
}