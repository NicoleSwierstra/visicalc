#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "parser.h"
#include "table.h"
#include "commands.h"

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

HANDLE  hConsole;

//console/display dimentions
int c_width = 7, c_height = 20;
//table dimentions
const int t_width = 256, t_height = 256;

static std::string getletter(int n) {
	std::string o = "";
	if (n < 26) {
		o = ('A' + n);
	}
	else {
		o += ('@' + (n / 26));
		o += ('A' + (n % 26));
	}
	return o;
}

void move_cursor() {
	switch (_getch()) {
	case ARROW_UP: //up
		if (Table::cursor.y > 0) Table::cursor.y--;
		break;
	case ARROW_DOWN: //down
		if (Table::cursor.y < t_height - 1) Table::cursor.y++;
		break;
	case ARROW_RIGHT: //right
		if (Table::cursor.x < t_width - 1) Table::cursor.x++;
		break;
	case ARROW_LEFT: //left
		if (Table::cursor.x > 0) Table::cursor.x--;
		break;
	}

	//moving around the screen
	Table::reset_view();
}

int main()
{
	if (Table::init() != 0) return -1;
	unsigned char input = 0;
	Table::render();

	//program loop
	while (1) {
		input = _getch();
		switch (input) {
		case 224: //arrows
			move_cursor();
			break;
		case 13: //enter
			Table::edit_cell();
			break;
		case 27: //esc
			break;
		case 46:
			commands::command();
			break;
		case 19: //ctrl+s... somehow?
			Table::save();
			break;
		case 15: //ctrl+o... somehow?
			Table::load();
			break;
		}
		
		Table::render();
		std::cout << (int)input << "\n";
	}

	getchar();
}