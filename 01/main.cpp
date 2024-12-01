#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


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
	}

	// Sort them.
	std::sort(first.begin(), first.end(), comp);
	std::sort(second.begin(), second.end(), comp);

	// Sum up diffs.
	for ( auto idx = 0; idx < first.size(); ++idx )
		sum += std::abs(first.at(idx) - second.at(idx));

	// Print res.
	std::cout << sum << '\n';

	return 0;
}
