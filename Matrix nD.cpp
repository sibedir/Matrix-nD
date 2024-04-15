//#define SIB_DEBUG_INTEGRAL_CAST_FULL

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
		auto sdf = std::vector{ 3, 5, 6 };

		constexpr auto mds0 = sib::TMultiDimParam<3, unsigned int>{ 3, 5, 6 };

		sib::TMultiDimParam<3, unsigned> mds1{ 3, 5, 6 };

		auto mds2 = sib::TMultiDimParam(3, 5, 6);

		int arr0[] { 5, 6, 2, 7 };
		/*
		sib::TMultiDimParam mds0(arr0);
		sib::TMultiDimParam mds1(arr1);
		
		sib::TMultiDimParam mds2(mds0);
		sib::TMultiDimParam mds3(mds1);

		sib::TMultiDimParam<5> mds4(ptr0);
		sib::TMultiDimParam<4> mds5(ptr1);
		*/
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