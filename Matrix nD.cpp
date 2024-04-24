//#define SIB_DEBUG_INTEGRAL_CAST_FULL
//#define SIB_DEBUG_MATRIX_FULL

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

constexpr auto QQQ() {
	return sib::MakeMatrix<int>( 4, 2, 3, 5, 4, 3 ).Data().size();
}

int main()
{
	try {
		{
			auto M1 = sib::MakeMatrix<short>(1, 2, 4, 6, 2, 2);
			auto M2 = sib::TMatrixND<6, short>(1, 2, 4, 6, 2, 2);

			auto& M3 = M1;
			auto M4 = M1;
			auto M5 = std::move(M1);
		} {
			constexpr auto val = QQQ();
		} {
			int arr[]{ 3, 5, 1, 6, 5 };
			std::array arr2 ( std::to_array(arr) );
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<5, int>(arr);
			std::cout << arr2[1];
		} {
			size_t arr[255]{ 3, 5, 1, 5, 999999999999 };
			auto M = sib::MakeMatrix<int>(arr);
		} {
			int arr[300]{ 3245, 5245, 1234, 223422343, 99999999 };
			auto M1 = sib::MakeMatrix<2, int>(arr); // 3 - bad_alloc, 4 - vector_to_long, 5 - overflow in Multiply
			auto M2 = sib::TMatrixND<2, int>(arr); // 3 - bad_alloc, 4 - vector_to_long, 5 - overflow in Multiply
		} {
			sib::TMatrixND<3, int> M(3, 2, 3);
			auto M1 = M;
			auto& M2 = M;
			decltype(auto) M3 = std::move(M);
			decltype(auto) M4 = std::move(M3);
			auto M3p = &M3;
			auto& M3pr = M3p;
			auto M4p = &M4;
			auto Mp = &M;
		} {
			constexpr auto s1 = sib::TMatrixND<3, int>(4,5,3).Data().size();
			constexpr int arr[]{ 3, 5, 2 };
			//constexpr auto s2 = std::move(sib::MakeMatrix<int>(arr)).Data().size();
		}
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