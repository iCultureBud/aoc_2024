#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


bool comp( int a, int b )
{
	return a <= b;
}


int main( int argc, char *argv[] ) {

	std::ifstream file(argv[1]);
	std::string line;

	int32_t sum { 0 };
	std::vector<int32_t> first;
	std::vector<int32_t> second;

	if ( ! file.is_open() )
		return -1;

	// Fill lists.
	while ( getline(file, line) ) {
		size_t space = line.find(' ');
		first.push_back( stoi(line.substr(0, space)) );
		second.push_back( stoi(line.substr(space, line.length() - 1)) );

		// nice shit from DrinoSan
		std::stringstream str { line };
		int32_t ln;
		int32_t rn;
		str >> ln >> rn;
	}

	// Sum up occurances.
	for ( auto num : first )
		sum += num * std::count( second.begin(), second.end(), num );

	// Print res.
	std::cout << sum << '\n';

	return 0;
}
