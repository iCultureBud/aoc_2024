#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( !file.is_open() )
		return -1;

	uint32_t sum { 0 };
	std::map<uint32_t, std::vector<uint32_t>> lookup;

	// Build up lookup map.
	while ( getline(file, line) )
	{
		if ( line.empty() )
			break;

		lookup[ std::stoi(line.substr(0, 2)) ].push_back( std::stoi(line.substr(3, 2)) );
	}

	// Loop through codes and check conditions.
	while ( getline(file, line) )
	{
		std::stringstream str { line };
		std::vector<uint32_t> currCodes;
		std::string curr;
		bool isValid { true };

		while( getline(str, curr, ',') )
			currCodes.push_back( std::stoi(curr) );

		for ( size_t idx = 0; idx < currCodes.size(); ++idx )
		{
			for ( uint32_t check : lookup[ currCodes[idx] ] )
			{
				auto it = std::find( currCodes.begin(), currCodes.end(), check );

				if ( it == currCodes.end() )
					continue;

				if ( it - currCodes.begin() > idx )
					continue;

				// If we get here the current check value is present BEFORE current code.
				// So something must be wrong.
				isValid = false;

				// Delete found false placed code.
				currCodes.erase( it );

				// We deleted something - so decrement idx.
				--idx;

				// Reinsert current check value behind current code.
				currCodes.insert( currCodes.begin() + idx + 1, check );
			}
		}

		if ( !isValid )
			sum += currCodes.at( currCodes.size() / 2 );
	}

	std::cout << sum << '\n';

	return 0;
}
