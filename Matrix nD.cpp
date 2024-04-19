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

int main()
{
	try {
		sib::TMatrix<int> M0 = (1, 2, 4, 6, 2, 2);
		//sib::TMatrix<int> M2(1, 2, 4, 6, 2, 2);

		int arr [] { 1, 2, 4, 6, 2, 2 };
		//std::array A1{ 1, 2, 4, 6, 2, 2 };
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