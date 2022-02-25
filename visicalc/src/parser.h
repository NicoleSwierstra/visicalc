#pragma once
#include <string>
#include <vector>

#define INSTR_SUM 0x0
#define INSTR_MIN 0x1
#define INSTR_MAX 0x2
#define INSTR_AVERAGE 0x3
#define INSTR_COUNT 0x4

template<typename T1, typename T2>
constexpr auto Min(T1 x, T2  y) { return (x < y) ? x : y; }
template<typename T1, typename T2>
constexpr auto Max(T1 x, T2  y) { return (x > y) ? x : y; }
constexpr bool isNumber(char c) { return c >= '0' && c <= '9'; }
constexpr bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
constexpr bool isLower(char c) { return c >= 'a' && c <= 'z'; }
constexpr bool isLetter(char c) { return isUpper(c) || isLower(c); }

struct coords {
	int x;
	int y;
};

extern coords getcoords(const char* s);

struct instruction {
	char type;
	std::vector<coords> cells;
	std::vector<int> nums;
};

class parser {
public:
	static std::string parse(std::string line);
	static std::string expression(std::string exp);
	static std::string expression_prime(std::string exp_p);
	static std::string term(std::string t);
	static std::string term_prime(std::string t_p);
	static std::string factor(std::string factor);

	static float instr_Sum(std::vector<coords> coordlist);
	static float instr_Average(std::vector<coords> coordlist);
	static float instr_Min(std::vector<coords> coordlist);
	static float instr_Max(std::vector<coords> coordlist);
	static int instr_Count(std::vector<coords> coordlist);
};