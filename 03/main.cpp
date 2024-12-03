#include <fstream>
#include <iostream>
#include <regex>
#include <string>


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	getline(file, line);
	uint64_t sum { 0 };
	std::regex search { "mul\\((\\d+),(\\d+)\\)" };
	std::regex_iterator<std::string::iterator> regIter ( line.begin(), line.end(), search );
	std::regex_iterator<std::string::iterator> lineEnd;

	while ( regIter != lineEnd )
	{
		if ( regIter->str(0) == "0" )
			break;

		int32_t multiplier = std::stoi( regIter->str(1) );
		int32_t multiplicand = std::stoi( regIter->str(2) );
		sum += multiplier * multiplicand;
		++regIter;
	}

	std::cout << sum << '\n';

	return 0;
}
