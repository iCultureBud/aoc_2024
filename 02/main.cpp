#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


enum MODE {
	NONE, DEC, INC
};

bool checkDiffs( int32_t diffPrev, int32_t diffNext, MODE mode )
{
	bool absDiffs = std::abs( diffPrev ) <= 3
			&& std::abs( diffNext ) <= 3
			&& std::abs( diffPrev ) > 0
			&& std::abs( diffNext ) > 0;

	switch ( mode )
	{
		case MODE::DEC:
			return absDiffs && diffPrev > 0 && diffNext > 0;
		case MODE::INC:
			return absDiffs && diffPrev < 0 && diffNext < 0;
		case MODE::NONE:
		default:
			return false;
	}
}


int main(int argc, char *argv[]) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	int32_t sum { 0 };
	while ( getline(file, line) ) {
		bool valid { true };
		MODE mode { MODE::NONE };
		int32_t prev { -1 };
		int32_t curr { -1 };
		int32_t next { -1 };
		std::stringstream sstream { line };

		sstream >> curr;
		while ( sstream )
		{
			sstream >> next;

			// Set mode of line.
			if ( mode == MODE::NONE )
				mode = curr - next >= 0 ? MODE::DEC : MODE::INC;

			// Excluding first and last iteration we can compare.
			if ( prev != -1 && next != -1 )
				if ( !checkDiffs(prev - curr, curr - next, mode) )
					valid = false;

			// Update vars.
			prev = curr;
			curr = next;
			next = -1;
		}

		if ( valid )
			++sum;
	}

	std::cout << sum << '\n';
	return 0;
}
