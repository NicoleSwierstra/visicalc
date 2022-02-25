#include "table.h"
#include <windows.h>
#include <iostream>
#include "input.h"

const static int width = 256, height = 256, c_width = 7, c_height = 20;
HANDLE Table::hConsole;
coords Table::cursor, Table::topleft;
entry* Table::table[height][width];
std::string filename = "saved_tables/";
std::string Table::current_action = "";

void Table::set_cursor_visibility(bool vis) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = vis;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int Table::init() {
	//this is hacky and cheaty and awful
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleMode(hConsole, 7);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 24;
	cfi.dwFontSize.Y = 42;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_DONTCARE;
	wcscpy_s(cfi.FaceName, L"terminal");
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	set_cursor_visibility(false);

	topleft = coords{ 0,0 };

	return 0;
}

char Table::checkvalue(int y, int x) {
	if (table[y][x] == nullptr) return 'N'; //if doesn't exist
	switch (table[y][x]->type) {
	case entry::_type::EXPRESSION:
		return 'E';
	case entry::_type::FLOAT_VALUE:
		return 'F';
	case entry::_type::INT_VALUE:
		return 'V';
	case entry::_type::LABEL:
		return 'L';
	}
}

void Table::print_cell(int y, int x) {
	std::string str;
	switch(checkvalue(y,x)){
	case 'N': 
		str = ""; 
		break;
	case 'E':
		str = "";
		break;
	case 'F':
		str = std::to_string(table[y][x]->value.f);
		break;
	case 'V':
		str = std::to_string(table[y][x]->value.i);
		break;
	case 'L':
		str = table[y][x]->label;
		break;
	}
	printf(
		"%9.9s",
		str.c_str()
	);
}

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

void Table::render() {
	std::cout << "\033[0;0H"; //cursor position 0,0, will overwrite

	c_filled();

	//top line
	printf("%2.2s%-3d (%c) %-*.*s\n",
		getletter(cursor.x).c_str(), //A-Z top left
		cursor.y, //1 - 99 top left
		checkvalue(cursor.y, cursor.x), //type of value I think?
		(c_width * 9) - 6, //min size
		(c_width * 9) - 6, //max size, should be same as above
		table[cursor.y][cursor.x]->label.c_str() //prints the string under the cursor
	);

	//2nd line
	printf("%-*.*s",
		(c_width * 9) - 2,
		(c_width * 9) - 2,
		current_action.c_str()
	);
	c_default();
	std::cout << "plenty\n"; //memory left
	std::cout << "\n"; //current input, see edit_cell()

	c_filled();
	std::cout << "   "; //3 leading characters on the char line
	for (int i = topleft.x; i < topleft.x + c_width; i++) {
		printf("   %2.2s    ", getletter(i).c_str()); //prints the columns
	}
	std::cout << "\n";

	//prints the rows and stuff in boxes
	for (int i = topleft.y; i < topleft.y + c_height; i++) {
		c_filled();
		printf("%3d", i); //print the y coord on left
		c_default();
		for (int j = topleft.x; j < topleft.x + c_width; j++) {
			if (i == cursor.y && j == cursor.x) {
				c_filled();
				print_cell(i, j);
				c_default();
			}
			else print_cell(i, j);
		}
		std::cout << "\n";
	}
}

void Table::reset_view()
{
	if (cursor.x < topleft.x) {
		topleft.x = cursor.x; }
	else if (cursor.x >= topleft.x + c_width) {
		topleft.x = cursor.x - c_width + 1;
	}
	if (cursor.y < topleft.y) {
		topleft.y = cursor.y;
	}
	else if (cursor.y >= topleft.y + c_height) {
		topleft.y = cursor.y - c_height + 1;
	}
}

void Table::save() {
	//current_action = "SAVE TABLE";
	//if (!getinput(filename)) return;
	//
	//FILE* f = fopen(filename.c_str(), "w");
	//
	//if (f == NULL) {
	//	current_action = "ERROR CREATING FILE! PRESS ENTER";
	//	getchar();
	//	return;
	//}
	//
	////iterate through every console position
	//for (int ty = 0; ty < 256; ty++) {
	//	for (int tx = 0; tx < 256; tx++) {
	//		if (table[ty][tx] != nullptr) {
	//			if (checkvalue(ty, tx) == 'E' || 'L')
	//			{
	//				fprintf(f, "%c%cS%s", tx, ty, table[ty][tx]->label.c_str());
	//				fputc(0, f);
	//			}
	//			if (checkvalue(ty, tx) == 'V') {
	//				fprintf(f, "%c%cI", tx, ty);
	//				
	//			}
	//				
	//			 //entries seperated through '\0'
	//		} // prints out x, y, and then the string in bianary
	//	}
	//}
	//
	//fclose(f);
	//current_action = "";
}

void Table::load() {
	//current_action = "LOAD TABLE";
	//if (!getinput(filename)) return;
	//
	//FILE* f = fopen(filename.c_str(), "r");
	//
	//if (f == NULL) {
	//	current_action = "FILE NOT FOUND! PRESS ENTER";
	//	getchar();
	//	return;
	//}
	//
	//int x = 0, y = 0, c = 0, i = 0;
	//while (c != EOF) { //while not at the end of file
	//	x = fgetc(f); y = fgetc(f);
	//	table[y][x] = ""; //reset table
	//	i++;
	//	c = fgetc(f);
	//	while (c && c != EOF) { //while not a null character
	//		table[y][x] += (char)c;
	//		c = fgetc(f);
	//	}
	//}
	//
	//fclose(f);
	//current_action = "";
}

void Table::edit_cell() {
	getinput(table[cursor.y][cursor.x]);
}