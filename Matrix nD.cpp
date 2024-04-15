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

		constexpr auto mds0 = sib::TMultiDimParam<3, unsigned int>{ 4, 5, 6 };

		sib::TMultiDimParam<3, unsigned> mds1{ 3, 5, 6 };

		//auto mds2 = sib::TMultiDimParam(3, 5, 6);

		int arr0[]{ 5, 6, 2, 7 };
		size_t arr1[]{ 5, 6, 2, 7 };
		
		constexpr auto asd = sizeof(arr0) / sizeof(std::remove_extent_t<decltype(arr0)>);

		auto ptr0 = &arr0[0];
		auto ptr1 = &arr1[0];

		//sib::TMultiDimParam mds3(arr0);
		sib::TMultiDimParam mds4(arr1);
		
		//sib::TMultiDimParam mds5(mds3);
		//sib::TMultiDimParam mds6(mds4);

		//sib::TMultiDimParam<5> mds7(ptr0);
		//sib::TMultiDimParam<4> mds8(ptr1);
	
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