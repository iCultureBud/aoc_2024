#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


enum MODE {
	NONE, DEC, INC
};

enum DIFF_t {
	INVALID, VALID, REMOVED
};


bool checkTuple( int32_t first, int32_t second, MODE& mode )
{
	int32_t diff = first - second;
	bool absDiffs = std::abs( diff ) <= 3 && std::abs( diff ) > 0;

	switch ( mode )
	{
		case MODE::DEC:
			return diff > 0 && absDiffs;
		case MODE::INC:
			return diff < 0 && absDiffs;
		case MODE::NONE:
		default:
			return false;
	}
}


DIFF_t checkDiffs( int32_t prev, int32_t curr, int32_t next, MODE& mode )
{
	// uint32_t diff = prev - curr;
	// bool isValid = checkTuple( prev, curr, mode );
	bool prevValid = checkTuple(prev, curr, mode); // true
	bool nextValid = checkTuple(curr, next, mode); // false
	bool interValid = checkTuple(prev, next, mode); // false

	if ( !prevValid && !nextValid && !interValid )
		return DIFF_t::INVALID;

	if ( prevValid && nextValid )
		return DIFF_t::VALID;

	return DIFF_t::REMOVED;
	// return checkTuple( curr, next , mode ) ? DIFF_t::VALID : DIFF_t::INVALID;
}


int main(int argc, char *argv[]) {

	std::ifstream file( argv[1] );
	std::string line;

	if ( !file.is_open() )
		return -1;

	int32_t sum { 0 };
	while ( getline(file, line) ) {
		int32_t idx { -1 };
		std::vector<bool> alrRmvd;
		bool valid { true };
		bool firstRmvd { false };
		bool lastRmvd { false };
		MODE mode { MODE::NONE };
		int32_t prev { -1 };
		int32_t curr { -1 };
		int32_t next { -1 };
		std::stringstream sstream { line };

		sstream >> curr;
		while ( sstream >> next )
		{
			bool prevRmvd { false };
			// Set mode of line.
			if ( mode == MODE::NONE )
				mode = curr - next >= 0 ? MODE::DEC : MODE::INC;

			// Excluding first and last iteration we can compare.
			if ( prev != -1 && next != -1 )
			{
				DIFF_t currRes = checkDiffs( prev, curr, next, mode );
				alrRmvd.push_back( currRes == DIFF_t::REMOVED );

				if ( currRes == DIFF_t::INVALID )
				{
					valid = false;
					break;
				}

				// std::cout << alrRmvd.at( idx ) << '\n';

				if ( currRes == DIFF_t::REMOVED )
				{
					if ( idx == 0 )
						firstRmvd = true;

					int32_t target = idx == 1 ? 0 : idx - 2;

					try {
						prevRmvd = alrRmvd.at( target );
					} catch ( const std::out_of_range& e ) {
						// Do nothing - lol.
					}

					if ( prevRmvd || !firstRmvd )
					{
						valid = false;
						break;
					}
				}

				std::cout << '[' << prev << ',' << curr << ',' << next << "] with res: " << currRes << '\n';
				std::cout << "AND prev: " << prevRmvd << '\n';
			}

			// Update vars.
			++idx;
			prev = curr;
			curr = next;
			next = -1;
		}

		std::cout << line << " is: " << valid << '\n';

		if ( valid )
			++sum;
	}

	std::cout << sum << '\n';
	return 0;
}
