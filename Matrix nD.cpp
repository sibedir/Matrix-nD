#define SIB_DEBUG_INTEGRAL_CAST_FULL

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>

#include "sib_matrix_nD.h"
#include "sib_exception.h"
#include "sib_checks.h"

template<size_t N>
constexpr auto ttt(const int(&arr)[N])
{
	return std::vector<int>(&arr[0], &arr[N-1]);
}

constexpr auto vvv0 = std::vector<int>{ 4,2,3,5,4,3 };



constexpr int arr[]{ 2, 3, 5, 7 };
constexpr auto vvv0 = std::vector<int>(4, 0);

int main()
{
	constexpr auto vvv1 = ttt(arr);

	try {
		std::array A1{ 1, 2, 4, 6, 2, 2 };

		auto M1 = sib::TMatrix<int, 6>( 1, 2, 4, 6, 2, 2 );
		//sib::TMatrix<int> M2(1, 2, 4, 6, 2, 2);
	}
	catch (const std::exception& exc) {
		std::cerr << sib::ExceptionInfo(exc) << std::endl;
	}

//THE_END:
	std::cout << std::endl << std::endl;
	std::cout << "  - THE END -  " << std::endl;
	std::cout << "press [Enter]...";
	std::cin.clear();
	//std::cin.get();
	std::cin.get();
	exit(0);
}