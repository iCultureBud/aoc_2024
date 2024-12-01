#include <fstream>
#include <iostream>
#include <string>


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	while ( getline(file, line) ) {
		std::cout << line << std::endl;
	}

	return 0;
}
