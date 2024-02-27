#pragma once

#include <map>
#include <vector>
#include <iostream>

using namespace std;

#pragma region Colors
#define BLACK "\x1B[38;5;232m"
#define DARK_GRAY "\x1B[38;5;237m"
#define GRAY "\x1B[38;5;244m"
#define LIGHT_GRAY "\x1B[38;5;249m"
#define WHITE "\x1B[38;5;255m"
#define DARK_RED "\x1B[38;5;124m"
#define RED "\x1B[38;5;196m"
#define DARK_ORANGE "\x1B[38;5;130m"
#define ORANGE "\x1B[38;5;208m"
#define DARK_YELLOW "\x1B[38;5;136m"
#define YELLOW "\x1B[38;5;226m"
#define LIME "\x1B[38;5;82m"
#define GREEN "\x1B[38;5;106m"
#define BLUE "\x1B[38;5;63m"
#define LIGHT_BLUE "\x1B[38;5;12m"
#define CYAN "\x1B[38;5;51m"
#define PINK "\x1B[38;5;219m"
#define MAGENTA "\x1B[38;5;199m"
#define PURPLE "\x1B[38;5;99m"
#define BROWN "\x1B[38;5;130m"

#define BG_BLACK "\x1B[48;5;232m"
#define BG_DARK_GRAY "\x1B[48;5;237m"
#define BG_GRAY "\x1B[48;5;244m"
#define BG_LIGHT_GRAY "\x1B[48;5;249m"
#define BG_WHITE "\x1B[48;5;255m"
#define BG_DARK_RED "\x1B[48;5;124m"
#define BG_RED "\x1B[48;5;196m"
#define BG_DARK_ORANGE "\x1B[48;5;130m"
#define BG_ORANGE "\x1B[48;5;208m"
#define BG_DARK_YELLOW "\x1B[48;5;136m"
#define BG_YELLOW "\x1B[48;5;226m"
#define BG_LIME "\x1B[48;5;82m"
#define BG_GREEN "\x1B[48;5;106m"
#define BG_BLUE "\x1B[48;5;63m"
#define BG_LIGHT_BLUE "\x1B[48;5;12m"
#define BG_CYAN "\x1B[48;5;51m"
#define BG_PINK "\x1B[48;5;219m"
#define BG_MAGENTA "\x1B[48;5;199m"
#define BG_PURPLE "\x1B[48;5;99m"
#define BG_BROWN "\x1B[48;5;130m"

#define COLOR_RESET "\033[0m"
#define UNDERLINE "\033[4m"
#define SWAP "\033[7m" // Swap background and foreground colors

// x => Color code between 0 and 255
#define COLOR(x) "\x1B[38;5;"<<x<<"m"
#define BG_COLOR(x) "\x1B[48;5;"<<x<<"m"
#pragma endregion

template <typename Key, typename Value>
static void Debug(const Key& _key, Value* _value)
{
	string _name = typeid(_value).name();
	_name = _name.substr(_name.find_first_of(' ') + 1, _name.length());
	_name = _name.substr(0, _name.find_first_of('*') - 1);
	//cout << RED << "[GarbageCollector]> " << DARK_ORANGE << "Removed " << _name << " with id '" << _key << "'!" << COLOR_RESET << endl;
}

template <typename Value>
static void Debug(Value* _value)
{
	string _name = typeid(_value).name();
	_name = _name.substr(_name.find_first_of(' ') + 1, _name.length());
	_name = _name.substr(0, _name.find_first_of('*') - 1);
	//cout << RED << "[GarbageCollector]> " << DARK_ORANGE << "Removed unregistered " << _name << "!" << COLOR_RESET << endl;
}