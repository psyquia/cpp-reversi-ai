#include "myhelper.h"

using namespace std;

map<char, int> index_map = {
	{ 'a', 1 }, 
	{ 'b', 2 },
	{ 'c', 3 },
	{ 'd', 4 },
	{ 'e', 5 },
	{ 'f', 6 },
	{ 'g', 7 },
	{ 'h', 8 },
};

map<int, char> letter_map = {
	{ 1, 'a'},
	{ 2, 'b'},
	{ 3, 'c'},
	{ 4, 'd'},
	{ 5, 'e'},
	{ 6, 'f'},
	{ 7, 'g'},
	{ 8, 'h'},
};

vector<vector<int>> directions = {
	{1, 0},
	{0, 1},
	{1, 1},
	{-1,0},
	{0,-1},
	{-1,-1},
	{1,-1},
	{-1,1}
};

string indexToStr(int x, int y){
	return string(1,letter_map[x+1]) + to_string(y+1);
}

vector<int> strToIndex(string str){
	return {index_map[str[0]]-1, stoi(str.substr(1,1),nullptr,10)-1};
}


string my_tolower(std::string s){
	string res = "";
	for(char c : s){
		res += tolower(c);
	}
	return res;
}