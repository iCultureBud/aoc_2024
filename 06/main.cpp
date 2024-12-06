#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void findPath( int32_t row, int32_t col, char direction,
			   uint64_t& sum, std::vector<std::string>& map );


int main( int argc, char *argv[] ) {

	std::ifstream file(argv[1]);
	std::string line;

	if ( ! file.is_open() )
		return -1;

	size_t idx { 0 };
	uint64_t sum { 0 };
	int32_t startRow;
	int32_t startCol;
	std::vector<std::string> map;

	// Build up the map and remember starting position.
	while ( getline(file, line) )
	{
		size_t start = line.find( '^' );
		if ( start != std::string::npos )
		{
			startRow = idx;
			startCol = start;
		}

		map.push_back( line );
		++idx;
	}

	findPath( startRow, startCol, 'u', sum, map );
	std::cout << sum << '\n';

	return 0;
}


//------------------------------------------------------------------------------
// Recursive function to find the path of the dude.
//
void findPath( int32_t row, int32_t col, char direction,
			   uint64_t& sum, std::vector<std::string>& map )
{
	// Check if we reached the end.
	if ( ( direction == 'u' && ( row < 0 || row - 1 < 0 ) )
	|| ( direction == 'r' && ( col > map[row].length() - 1 || col + 1 > map[row].length() - 1 ) )
	|| ( direction == 'd' && ( row > map.size() - 1 || ( row + 1 > map.size() - 1 ) ) )
	|| ( direction == 'l' && ( col < 0 || col - 1 < 0 ) ) )
	{
		++sum;
		return;
	}

	// Mark as visited and count one point to path.
	if ( map[row][col] != 'X' )
	{
		map[row][col] = 'X';
		++sum;
	}

	// Send to find next spot.
	// Go up or right.
	if ( direction == 'u' )
	{
		if ( map[row - 1][col] != '#' )
			return findPath( row - 1, col, 'u', sum, map );
		else
			return findPath( row, col + 1, 'r', sum, map );
	}

	// Go right or down.
	if ( direction == 'r' )
	{
		if ( map[row][col + 1] != '#' )
			return findPath( row, col + 1, 'r', sum, map );
		else
			return findPath( row + 1, col, 'd', sum, map );
	}

	// Go down or left.
	if ( direction == 'd' )
	{
		if ( map[row + 1][col] != '#' )
			return findPath( row + 1, col, 'd', sum, map );
		else
			return findPath( row, col - 1, 'l', sum, map );
	}

	// Go left or up.
	if ( direction == 'l' )
	{
		if ( map[row][col - 1] != '#' )
			return findPath( row, col - 1, 'l', sum, map );
		else
			return findPath( row - 1, col, 'u', sum, map );
	}

	return;
}
