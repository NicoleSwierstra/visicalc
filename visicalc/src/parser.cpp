#include "parser.h"
#include "table.h"

/* Parser design (old, irrelevent, don't read)
 * 
 * example statement:
 * SUM(B0 -- B4, B6);
 * '--' = range (the origional visicalc used elipses, however this seemed like a better choice)
 * ',' = individual number
 * --------------------------------------
 * 
 * operations:
 * 
 * SUM: sum each location in parenthesis
 * MIN: get minimum value from locations
 * MAX: get maximum value from locations
 * AVERAGE: average each location in parenthesis
 * COUNT: non-blank arguments in range
 * 
 * --------------------------------------
 * 
 * lexical tree:
 * 
 * operation -> expression
 * expression  -> term expression`
 * expression` -> , term expression` | null
 * term        -> factor term`
 * term`       -> - factor term` | null
 * factor       -> coord | value | @operation
 * 
 * ---------------------------------------
 * 
 * example parse:
 * 
 */

std::string remspace(std::string s) {
	std::string s1;
	int i1 = s.length();
	int i = 0;
	while (i < i1) {
		if (s[i] != ' ') s1 += s[i];
		i++;
	}
	return s1;
}

coords getcoords(const char* s) {
	int x = 0, y = 0;
	int firstnum = 0;
	while (s[firstnum] && isLetter(s[firstnum])) 
		firstnum++;
	sscanf(s + firstnum, "%d", &y);
	for (int i = firstnum - 1; i >= 0; i--) {
		int place = (firstnum - i - 1);
		x += (s[i] - '@') * Max((26 * place), 1);
	}
	return { x - 1, y };
}

std::vector<coords> getrange(std::string str) {
	size_t s1 = str.find("--");
	coords c1 = getcoords(str.substr(0, s1).c_str()), 
		c2 = getcoords(str.substr(s1 + 2, str.length()).c_str());
	std::vector<coords> c;
	for (int y = Min(c1.y, c2.y); y <= Max(c1.y, c2.y); y++) {
		for (int x = Min(c1.x, c2.x); x <= Max(c1.x, c2.x); x++) {
			c.push_back(coords{x, y});
		}
	}
	return c;
}

int getInt(std::string str) {
	int i;
	sscanf(str.c_str(), "%d", &i);
	return i;
}

std::string parser::parse(std::string line)
{
	return expression(remspace(line));
}

std::string parser::expression(std::string exp) {
	int instr_type = -1;
	int beg = 0;
	if (exp.substr(0, 4) == "@SUM") {
		instr_type = INSTR_SUM;
		beg += 4;
	}
	else if (exp.substr(0, 4) == "@MIN") {
		instr_type = INSTR_MIN;
		beg += 4;
	}
	else if (exp.substr(0, 4) == "@MAX") {
		instr_type = INSTR_MAX;
		beg += 4;
	}
	else if (exp.substr(0, 8) == "@AVERAGE") {
		instr_type = INSTR_AVERAGE;
		beg += 8;
	}
	else if (exp.substr(0, 6) == "@COUNT") {
		instr_type = INSTR_COUNT;
		beg += 6;
	}
	beg++;
	if (instr_type == -1)
		return "error! not a valid instruction on char " + std::to_string(beg);
	int end = exp.length();
	while (exp[end] != ')') end--;
	std::vector<coords> c = getrange(exp.substr(beg, end - beg));
	switch (instr_type) {
	case INSTR_SUM: return std::to_string(instr_Sum(c));
	case INSTR_MIN: return std::to_string(instr_Min(c));
	case INSTR_MAX: return std::to_string(instr_Max(c));
	case INSTR_AVERAGE: return std::to_string(instr_Average(c));
	case INSTR_COUNT: return std::to_string(instr_Count(c));
	}
}

std::string parser::expression_prime(std::string exp_p)
{
	return "";
}

std::string parser::term(std::string t)
{
	return "";
}

std::string parser::term_prime(std::string t_p)
{
	return "";
}

std::string parser::factor(std::string fac)
{
	return "";
}

//these should all be self explanatory
float parser::instr_Sum(std::vector<coords> coordlist)
{
	float base = 0;
	for (int i = 0; i < coordlist.size(); i++) {
		if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::INT_VALUE)
			base += Table::table[coordlist[i].y][coordlist[i].x]->value.i;
		else if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::FLOAT_VALUE)
			base += Table::table[coordlist[i].y][coordlist[i].x]->value.f;
	}
	return base;
}

float parser::instr_Average(std::vector<coords> coordlist)
{
	float base = 0;
	int count = 0;
	for (int i = 0; i < coordlist.size(); i++) {
		if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::INT_VALUE) {
			base += Table::table[coordlist[i].y][coordlist[i].x]->value.i;
			count++;
		}
		else if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::FLOAT_VALUE) {
			base += Table::table[coordlist[i].y][coordlist[i].x]->value.f;
			count++;
		}
			
	}
	return base / count;
}

float parser::instr_Min(std::vector<coords> coordlist)
{
	float base = INFINITY;
	for (int i = 0; i < coordlist.size(); i++) {
		if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::INT_VALUE) {
			base = Min(base, Table::table[coordlist[i].y][coordlist[i].x]->value.i);
		}
		else if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::FLOAT_VALUE) {
			base = Min(base, Table::table[coordlist[i].y][coordlist[i].x]->value.f);
		}
	}
	return base;
}

float parser::instr_Max(std::vector<coords> coordlist)
{
	float base = -INFINITY;
	for (int i = 0; i < coordlist.size(); i++) {
		if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::INT_VALUE) {
			base = Max(base, Table::table[coordlist[i].y][coordlist[i].x]->value.i);
		}
		else if (Table::table[coordlist[i].y][coordlist[i].x]->type == entry::_type::FLOAT_VALUE) {
			base = Max(base, Table::table[coordlist[i].y][coordlist[i].x]->value.f);
		}
	}
	return base;
}

int parser::instr_Count(std::vector<coords> coordlist)
{
	int c = 0;
	for (int i = 0; i < coordlist.size(); i++) {
		if (Table::table[coordlist[i].y][coordlist[i].x] != nullptr)
			c++;
	}
	return c;
}
