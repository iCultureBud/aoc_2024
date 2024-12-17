#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>


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
static int64_t walls { 0 };
static DIR direction { DIR::UP };
static std::vector<std::string> map;

static std::map<uint8_t, char> dirMap {
	{ 0, '|' }, // DIR::UP
	{ 2, '|' }, // DIR::DOWN
	{ 1, '-' }, // DIR::RIGHT
	{ 3, '-' }, // DIR::LEFT
	{ 4, '+' }, // Corner - needs for context
};


// Forward decl.
void findPath();
bool findWall();
int32_t findWalls();
int32_t findUpWall( int32_t pos );
int32_t findRightWall( int32_t pos );
int32_t findDownWall( int32_t pos );
int32_t findLeftWall( int32_t pos );


//------------------------------------------------------------------------------
// Main chaos.
int main( int argc, char *argv[] )
{
	std::ifstream file(argv[1]);
	std::string line;

	if ( !file.is_open() )
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
	// for ( auto row : map )
	// 	std::cout << row << '\n';
	// std::cout << "------------------------ " << sum << '\n';

	std::cout << sum << '\n';


	return 0;
}


//------------------------------------------------------------------------------
// Recursive function to find the path of the dude.
//
void findPath()
{
	// Mark as visited and count one point to path.
	// if ( map[ row ][ col ] == '.' )
	// 	++sum;
	// if ( sum == 6 )
	// 	return;

	// Return if we can't go any further.
	if ( direction == DIR::UP && row - 1 < 0 )
		return;
	if ( direction == DIR::RIGHT && col + 1 > map[row].length() - 1 )
		return;
	if ( direction == DIR::DOWN && row + 1 > map.size() - 1 )
	{
		std::cout << "REACHING END BECAUSE: row = " << row << '\n';
		return;
	}
	if ( direction == DIR::LEFT && col - 1 < 0 )
		return;

	// Set next direction.
	// if ( row == 6 && col == 6 && sum > 0 )
	// {
	// 	std::cout << "WALL: " << findWall() << '\n';
	// 	std::cout << "DIR: " << static_cast<int32_t>( direction ) << '\n';
	// 	return;
	// }
	std::cout << "HELP: " << row << ", " << col << '\n';
	switch( direction )
	{
		case DIR::UP:
		{
			// Found wall
			if ( map[row - 1][col] == '#' )
			{
				map[row][col] = dirMap[4];
				++walls; ++direction; ++col;
			}

			// Near end
			else if ( row - 1 == 0 )
			{
				// if ( !findWall() )
				// 	return;

				map[row][col] = dirMap[4];
				++sum; ++direction; ++col;
			}

			// Can go
			else
			{
				if ( map[row][col] != '-' )
				{
					map[row][col] = dirMap[ static_cast<int32_t>( direction ) ];
				}
				else
				{
					sum += findWall();
					map[row - 1][col] = '0';
					map[row][col] = dirMap[4];
				}

				--row;
			}

			break;
		}

		case DIR::RIGHT:
		{
			// Found wall
			if ( map[row][col + 1] == '#' )
			{
				map[row][col] = dirMap[4];
				++walls; ++direction; ++row;
			}

			// Near end
			else if ( col + 1 == map[row].length() - 1 )
			{
				if ( !findWall() )
					return;

				map[row][col] = dirMap[4];
				++sum; ++direction; ++row;
			}

			// Can go
			else
			{
				if ( map[row][col] != '|' )
				{
					map[row][col] = dirMap[ static_cast<int32_t>( direction ) ];
				}
				else
				{
					sum += findWall();
					map[row][col + 1] = '0';
					map[row][col] = dirMap[4];
				}

				++col;
			}

			break;
		}

		case DIR::DOWN:
		{
			// Found wall
			if ( map[row + 1][col] == '#' )
			{ 
				// std::cout << "FOUND WALL AT: " << row << ", " << col << '\n';
				map[row][col] = dirMap[4];
				++walls; ++direction; --col;
			}

			// Near end
			else if ( row + 1 == map.size() - 1 )
			{
				std::cout << "REACHING ENDEND" << '\n';
				if ( !findWalls() )
					return;

				map[row][col] = dirMap[4];
				++sum; ++direction; --col;
			}

			// Can go
			else
			{
				std::cout << "WHY AM I HERE: " << row << ", " << col << '\n';
				if ( map[row][col]	!= '-' )
				{
					// std::cout << "FOUND NOTHING" << '\n';
					map[row][col] = dirMap[ static_cast<int32_t>( direction ) ];
				}
				else
				{
					// std::cout << "FOUND CROSS" << '\n';
					sum += findWall();
					map[row + 1][col] = '0';
					map[row][col] = dirMap[4];
				}

				++row;
			}

			break;
		}

		case DIR::LEFT:
		{
			// Found wall
			if ( map[row][col - 1] == '#' )
			{
				map[row][col] = dirMap[4];
				++walls; ++direction; --row;
			}

			// Near end
			else if ( col - 1 == 0 )
			{
				if ( !findWall() )
					return;

				map[row][col] = dirMap[4];
				++sum; ++direction; --row;
			}

			// Can go
			else
			{
				if ( map[row][col] != '|' )
				{
					map[row][col] = dirMap[ static_cast<int32_t>( direction ) ];
				}
				else
				{
					sum += findWall();
					map[row][col - 1] = '0';
					map[row][col] = dirMap[4];
				}

				--col;
			}

			break;
		}

		case DIR::WRAP_UP:
		default:
			break;
	}

	for ( auto row : map )
		std::cout << row << '\n';
	std::cout << "------------------------ " << walls << " : " << sum << '\n';

	return findPath();
}


int32_t findWalls()
{
	int32_t pos;

	if ( direction == DIR::DOWN )
	{
		pos = findLeftWall( col );
		if ( !pos )
			return pos;

		pos = findUpWall( pos );
		if ( !pos )
			return pos;

		pos = findRightWall( pos );
		return pos;
	}

	return -1;
}

int32_t findUpWall( int32_t pos )
{
	// Look up for wall.
	for ( int32_t idx = pos; idx >= 0; --idx )
		if ( map[idx][col] == '#' )
			return idx - 1;

	return -1;
}

int32_t findRightWall( int32_t pos )
{
	// Look right for wall.
	for ( int32_t idx = pos; idx < map[row].length(); ++idx )
		if ( map[row][idx] == '#' )
			return idx - 1;

	return -1;
}

int32_t findDownWall( int32_t pos )
{
	// Look down for wall.
	for ( int32_t idx = pos; idx < map.size(); ++idx )
		if ( map[idx][col] == '#' )
			return idx - 1;

	return -1;
}

int32_t findLeftWall( int32_t pos )
{
	// Look left for wall.
	for ( int32_t idx = pos; idx >= 0; --idx )
		if ( map[row][idx] == '#' )
			return idx - 1;

	return -1;
}



bool findWall()
{
	if ( walls % 3 != 0 )
		return false;

	// Look right for wall.
	if ( direction == DIR::UP )
		for ( int32_t idx = col; idx < map[row].length(); ++idx )
			if ( map[row][idx] == '#' )
				return true;

	// Look down for wall.
	if ( direction == DIR::RIGHT )
		for ( int32_t idx = row; idx < map.size(); ++idx )
			if ( map[idx][col] == '#' )
				return true;

	// Look left for wall.
	if ( direction == DIR::DOWN )
		for ( int32_t idx = col; idx >= 0; --idx )
			if ( map[row][idx] == '#' )
				return true;

	// Look up for wall.
	if ( direction == DIR::LEFT )
		for ( int32_t idx = row; idx >= 0; --idx )
			if ( map[idx][col] == '#' )
				return true;

	return false;
}
