#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


enum class DIR
{
   UP,
   RIGHT,
   DOWN,
   LEFT,
   WRAP_UP
};

// NICE shit from DrinoSan
DIR& operator++( DIR& dir )
{
   dir =  static_cast<DIR>( ( static_cast<int32_t>( dir ) + 1 ) %
                                  static_cast<int32_t>( DIR::WRAP_UP ) );
   return dir;
}


// Declare needed static vars.
static uint64_t sum { 0 };
static int32_t row { 0 };
static int32_t col { 0 };
static DIR direction { DIR::UP };
static std::vector<std::string> map;


// Forward decl.
void findPath();


//------------------------------------------------------------------------------
// Main chaos.
int main( int argc, char *argv[] )
{
	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	size_t idx { 0 };
	// Build up the map and remember starting position.
	while ( getline(file, line) )
	{
		size_t start = line.find( '^' );
		if ( start != std::string::npos )
		{
			row = idx;
			col = start;
		}

		map.push_back( line );
		++idx;
	}

	// Send this guy on an adventure :]
	findPath();
	std::cout << sum << '\n';

	return 0;
}


//------------------------------------------------------------------------------
// Recursive function to find the path of the dude.
//
void findPath()
{
	// Mark as visited and count one point to path.
	if ( map[row][col] != 'X' )
	{
		map[row][col] = 'X';
		++sum;
	}

	// Return if we can't go any further.
	if ( ( direction == DIR::UP && row - 1 < 0 )
		|| ( direction == DIR::RIGHT && col + 1 > map[row].length() - 1 )
		|| ( direction == DIR::DOWN && row + 1 > map.size() - 1 )
		|| ( direction == DIR::LEFT && col - 1 < 0 ) )
			return;

	// Set next direction.
	switch( direction )
	{
		case DIR::UP:
		{
			if ( map[row - 1][col] != '#' ) { --row; }
			else { ++col; ++direction; }
			break;
		}

		case DIR::RIGHT:
		{
			if ( map[row][col + 1] != '#' ) { ++col; }
			else { ++row; ++direction; }
			break;
		}

		case DIR::DOWN:
		{
			if ( map[row + 1][col] != '#' ) { ++row; }
			else { --col; ++direction; }
			break;
		}

		case DIR::LEFT:
			if ( map[row][col - 1] != '#' ) { --col; }
			else { --row; ++direction; }
			break;

		case DIR::WRAP_UP:
		default:
			break;
	}

	return findPath();
}
