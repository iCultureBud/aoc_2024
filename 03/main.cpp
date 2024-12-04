#include <fstream>
#include <iostream>
#include <regex>
#include <string>


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	uint64_t sum { 0 };
	bool enabled { true };
	std::regex search { "(do\\(\\)|don't\\(\\)|mul\\((\\d+),(\\d+)\\))" };
	while ( getline(file, line) )
	{
		std::regex_iterator<std::string::iterator> regIter ( line.begin(), line.end(), search );
		std::regex_iterator<std::string::iterator> lineEnd;

		while ( regIter != lineEnd )
		{
			if ( regIter->str(0) == "0" )
				break;

			if ( regIter->str(0) == "don't()" ) { enabled = false; }
			else if ( regIter->str(0) == "do()" ) { enabled = true; }
			else
			{
				if ( enabled )
				{
					int32_t multiplier = std::stoi( regIter->str(2) );
					int32_t multiplicand = std::stoi( regIter->str(3) );
					sum += multiplier * multiplicand;
				}
			}

			++regIter;
		}
	}

	std::cout << sum << '\n';

	return 0;
}
