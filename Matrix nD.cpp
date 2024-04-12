#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>

#include "sib_matrix_nD.h"
#include "sib_exception.h"

int main()
{
	try {
		constexpr auto asdfgd = std::unsigned_integral<uint16_t>;

		size_t arr0[5]{ 1, 9, 4, 3, 8 };
		int arr1[5]{ 6, 2, 2, 9, 7 };

		sib::TMultiDimParam<5, unsigned char> mds0(std::move(arr0));
		sib::TMultiDimParam<5> mds1(std::move(arr1));
		sib::TMultiDimParam<5> mds2(std::move(mds0));

		int d = 8;
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