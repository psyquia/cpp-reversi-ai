#pragma once
#include <string>
#include <map>
#include <vector>

extern std::map<char, int> index_map;

extern std::map<int, char> letter_map;

extern std::vector<std::vector<int>> directions;

extern std::string indexToStr(int x, int y);

extern std::vector<int> strToIndex(std::string str);

extern std::string my_tolower(std::string s); 
