#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

constexpr int32_t MAX_CHARS { 3 };
constexpr std::string SEARCH { "XMAS" };
constexpr std::string REV_SEARCH { "SAMX" };
int32_t countLR(int32_t inner, bool canL, bool canR, std::string& line );
int32_t countHV(int32_t outer, int32_t inner, bool canU, bool canD,
				bool canL, bool canR, std::vector<std::string>& map );


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;
	std::vector<std::string> map;

	if ( ! file.is_open() )
		return -1;

	while ( getline(file, line) )
		map.push_back( line );

	int64_t sum { 0 };
	bool canU { false };
	bool canD { false };
	bool canL { false };
	bool canR { false };

	for ( int32_t outer = 0; outer < map.size(); ++outer )
	{
		bool found { false };
		canU = outer >= MAX_CHARS;
		canD = outer <= map.size() - 4;

		for ( int32_t inner = 0; inner < map.at(outer).length(); ++inner )
		{
			canL = inner >= MAX_CHARS;
			canR = inner <= map.at(outer).length() - 4;

			if ( map.at(outer)[inner] != 'X' )
				continue;

			if ( canL || canR )
				sum += countLR(inner, canL, canR, map.at(outer));

			if ( canU || canD )
				sum += countHV(outer, inner, canU, canD, canL, canR, map);
		}
	}

	std::cout << sum << '\n';
	return 0;
}


//------------------------------------------------------------------------------
// Count matches in same line.
//
int32_t countLR(int32_t inner, bool canL, bool canR, std::string& line )
{
	bool foundL { 0 };
	bool foundR { 0 };

	if ( canL )
		foundL = line.substr(inner - MAX_CHARS, MAX_CHARS + 1) == REV_SEARCH;
	if ( canR )
		foundR = line.substr(inner, MAX_CHARS + 1) == SEARCH;

	return foundL + foundR;
}


//------------------------------------------------------------------------------
// Count matches in up(left/right) and down(left/right).
//
int32_t countHV(int32_t outer, int32_t inner, bool canU, bool canD,
				bool canL, bool canR, std::vector<std::string>& map )
{
	bool foundU { false };
	bool foundD { false };
	bool foundUL { false };
	bool foundUR { false };
	bool foundDL { false };
	bool foundDR { false };

	if ( canU )
	{
		foundU = std::string("")
			+ map[outer][inner]
			+ map[outer - 1][inner]
			+ map[outer - 2][inner]
			+ map[outer - MAX_CHARS][inner] == SEARCH;

		if ( canL )
			foundUL = std::string("")
				+ map[outer][inner]
				+ map[outer - 1][inner - 1]
				+ map[outer - 2][inner - 2]
				+ map[outer - MAX_CHARS][inner - MAX_CHARS] == SEARCH;

		if ( canR )
			foundUR = std::string("")
				+ map[outer][inner]
				+ map[outer - 1][inner + 1]
				+ map[outer - 2][inner + 2]
				+ map[outer - MAX_CHARS][inner + MAX_CHARS] == SEARCH;
	}

	if ( canD )
	{
		foundD = std::string("")
			+ map[outer][inner]
			+ map[outer + 1][inner]
			+ map[outer + 2][inner]
			+ map[outer + MAX_CHARS][inner] == SEARCH;

		if ( canL )
			foundDL = std::string("")
				+ map[outer][inner]
				+ map[outer + 1][inner - 1]
				+ map[outer + 2][inner - 2]
				+ map[outer + MAX_CHARS][inner - MAX_CHARS] == SEARCH;

		if ( canR )
			foundDR = std::string("")
				+ map[outer][inner]
				+ map[outer + 1][inner + 1]
				+ map[outer + 2][inner + 2]
				+ map[outer + MAX_CHARS][inner + MAX_CHARS] == SEARCH;
	}

	return foundUL + foundU + foundUR + foundDL + foundD + foundDR;
}
