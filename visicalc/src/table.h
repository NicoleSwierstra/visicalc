#pragma once
#include <string>
#include <Windows.h>
#include "parser.h"

struct entry {
	enum class _type {
		LABEL = 0, 
		INT_VALUE = 1, 
		FLOAT_VALUE = 2, 
		EXPRESSION = 3
	} type;
	union _value {
		int i;
		float f;
	} value;
	std::string label;
};

class Table {
	static HANDLE hConsole;
public:
	const static int width = 256, height = 256, c_width = 7, c_height = 20;
	static void c_filled() { printf("\033[30;42m"); } //black text on green bg
	static void c_default() { printf("\033[40;32m"); } //green text on black bg
	static coords cursor, topleft;
	static entry* table[height][width];
	static std::string current_action;

	static int init();
	static void set_cursor_visibility(bool vis);
	static char checkvalue(int y, int x);
	static void print_cell(int y, int x);
	static void edit_cell();
	static void render();
	static void reset_view();
	static void save();
	static void load();
};