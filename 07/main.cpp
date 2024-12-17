#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <chrono>


// enum OP { ADD, MUL };

// Forward decl
uint64_t sumUp( int32_t firstIdx, int32_t secondIdx, std::vector<int32_t>& operands );


int main( int argc, char *argv[] )
{
	auto const start = std::chrono::high_resolution_clock::now();
	std::ifstream file( argv[1] );
	std::string line;

	if ( !file.is_open() )
		return -1;

	uint64_t sum { 0 };
	uint64_t lineIdx { 0 };

	while ( getline(file, line) )
	{
		std::string check;
		std::string operand;
		std::vector<int32_t> operands;
		std::stringstream strs { line };

		// Get current equation result.
		strs >> check;
		check.erase(check.length() - 1);

		while ( strs >> operand )
			operands.push_back( std::stoi(operand) );

		uint64_t iCheck = std::stoi( check );
		uint64_t currSum { 0 };
		uint64_t currProd { 1 };

		// Check if one operator is enough to fullfill equation.
		for ( int32_t curr : operands ) {
			currSum += curr;
			currProd *= curr;
		}

		if ( currSum == iCheck || currProd == iCheck )
		{
			sum += iCheck;
			continue;
		}

		// We need to switch one operator - so let's start looooping.
		// std::vector<OP> operators( operands.size() - 1, OP::ADD );
		std::cout << line << '\n';
		for( int32_t idx = 1; idx < operands.size() - 1; ++idx )
		{
			//    0  1   2   3
			// [ 11  6  16  20 ]
			int32_t first = idx - 1;
			int32_t second = idx;
			
			uint64_t currRes = sumUp( first, second, operands );
			// std::cout << " RESULT: " << currRes << '\n';
			if ( iCheck != currRes )
				continue;

			sum += iCheck;
			break;
		}
	}

	std::cout << sum << '\n';

	auto const end = std::chrono::high_resolution_clock::now();
	auto const duration =
		std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << "us" << '\n';
	return 0;
}


uint64_t sumUp( int32_t firstIdx, int32_t secondIdx, std::vector<int32_t>& operands )
{
	uint64_t res { 0 };

	// Sum up left part.
	for ( size_t idx = 0; idx <= firstIdx; ++idx )
		res += operands[idx];

	// std::cout << " FIRSIDX: " << firstIdx << '\n';
	// std::cout << " LEFT: " << res << '\n';

	// Multiply by current target.
	res *= operands[secondIdx];
	// std::cout << " SECIDX: " << secondIdx << '\n';
	// std::cout << " PROD: " << res << '\n';

	// Sum up right part to product.
	for ( size_t idx = secondIdx + 1; idx < operands.size(); ++idx )
		res += operands[idx];

	// std::cout << " FULL: " << res << '\n';
	return res;
}
