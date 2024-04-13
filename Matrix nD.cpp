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

int main()
{
	try {
		int arr0[5]{ 1, 9, 4, 3, 8 };
		size_t arr1[4]{ 6, 2, 2, 9 };

		int* ptr0 = arr0;
		size_t* ptr1 = arr1;

		using arr_t = decltype(arr0);
		constexpr auto b0 = std::is_array_v<arr_t>;
		constexpr auto b1 = sib::chk::integral_array<arr_t>;
		constexpr auto b2 = sib::chk::integral_array_N<arr_t, 5>;
		constexpr auto b3 = sib::chk::integral_pointer<arr_t>;

		decltype(main) dfgd;

		auto sdf = std::vector{ 3, 5, 6 };
		auto mds = sib::TMultiDimParam{ 3, 4, 6 };

		sib::TMultiDimParam mds0(arr0);
		sib::TMultiDimParam mds1(arr1);
		
		sib::TMultiDimParam mds2(mds0);
		sib::TMultiDimParam mds3(mds1);

		sib::TMultiDimParam<5> mds4(ptr0);
		sib::TMultiDimParam<4> mds5(ptr1);

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