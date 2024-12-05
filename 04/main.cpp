#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

constexpr uint8_t MAX_CHARS { 1 };
constexpr std::string SEARCH { "MAS" };
constexpr std::string REV_SEARCH { "SAM" };
bool countAround(int32_t outer, int32_t inner, std::vector<std::string>& map );


int main( int argc, char *argv[] ) {

	std::ifstream file( argv[1] );
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
		canD = outer <= map.size() - 2;

		for ( int32_t inner = 0; inner < map[outer].length(); ++inner )
		{
			canL = inner >= MAX_CHARS;
			canR = inner <= map[outer].length() - 2;

			if ( map[outer][inner] != 'A' )
				continue;

			if ( canU && canR && canD && canL )
				if ( countAround(outer, inner, map) )
					++sum;
		}
	}

	std::cout << sum << '\n';
	return 0;
}


//------------------------------------------------------------------------------
// Count matches around the 'A's.
//
bool countAround(int32_t outer, int32_t inner, std::vector<std::string>& map )
{
	char foundUL = map[outer - MAX_CHARS][inner - MAX_CHARS];
	char foundDR = map[outer + MAX_CHARS][inner + MAX_CHARS];
	std::string LR { foundUL, map[outer][inner], foundDR };
	if ( LR != SEARCH && LR != REV_SEARCH )
		return false;

	char foundUR = map[outer - MAX_CHARS][inner + MAX_CHARS];
	char foundDL = map[outer + MAX_CHARS][inner - MAX_CHARS];
	std::string RL { foundUR, map[outer][inner], foundDL };
	if ( RL != SEARCH && RL != REV_SEARCH )
		return false;

	return true;
}
