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
#include "sib_utilities.h"

int main()
{
	try {
		auto M0 = sib::TMatrix<short>(1, 2, 4, 6, 2, 2);

		int arr1[]{ 3, 5, 1, 6, 5, 5 };
		auto M1 = sib::TMatrix<int>(arr1);

		size_t arr2[255]{ 3, 5, 1, 5, 999999999999 };
		auto M2 = sib::TMatrix<int>(arr2);

		int arr3[]{ 32345, 52345, 1234, 5345, 9999};
		auto M3 = sib::TMatrix<int>(arr3);
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