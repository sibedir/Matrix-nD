#define SIB_DEBUG_MATRIX_FULL
#define SIB_DEBUG_INTEGRAL_CAST_FULL

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>

#include "sib_utilities.h"
#include "sib_exception.h"
#include "sib_matrix_nD.h"

constexpr auto QQQ() {
	return sib::MakeMatrix<int>( 4, 2, 3, 5, 4, 3 ).Data().size();
}

int main()
{
	try {
		{
			auto M1 = sib::MakeMatrix<short>(1, 2, 4, 6, 2, 2);
			auto M2 = sib::TMatrixND<short, 6>{ 1, 2, 4, 6, 2, 2 };
			auto& M3 = M1;
			auto M4 = M1;
			auto M5 = std::move(M1);
	    }{
			constexpr auto val = QQQ();
		}{
			int arr[]{ 3, 5, 1, 6, 5 };
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			std::array arr{ 1, 2, 3, 2, 1 };
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			int arr[]{ 2, 3, 4, 5 };
			sib::TMatrixND<int, 4> M =arr;

			int i = 10;
			std::vector<int> V;
			//V = i;
		}{
			size_t arr[255]{ 3, 5, 1, 5, 999999999999 };
			auto M = sib::MakeMatrix<int>(arr);
		}{
			int arr[300]{ 3245, 5245, 1234, 223422343, 99999999 };
			auto M1 = sib::MakeMatrix<int, 2>(arr); // 3 - bad_alloc, 4 - vector_to_long, 5 - overflow in Multiply
			auto M2 = sib::TMatrixND<int, 2>(arr); // 3 - bad_alloc, 4 - vector_to_long, 5 - overflow in Multiply
		}{
			sib::TMatrixND<int, 3> M(3, 2, 3);
			auto M1 = M;
			auto& M2 = M;
			decltype(auto) M3 = std::move(M);
			decltype(auto) M4 = std::move(M3);
			auto M5 = M3;
			auto& M6 = M3;
			auto M3p = &M3;
			auto& M3pr = M3p;
			auto M4p = &M4;
			auto Mp = &M;
		}{
			constexpr auto s0 = QQQ();
			constexpr auto s1 = sib::TMatrixND<int, 3>(4, 5, 3).Data().size();
			constexpr int arr[]{ 3, 5, 2 };
			constexpr auto s2 = std::move(sib::MakeMatrix<int>(arr)).Data().size();
		}{
			auto M = sib::MakeMatrix<short>(1, 2, 4, 6, 2, 2);
			sib::TMatrix<short>& Mref = M;
			sib::TMatrix<short>* Mptr = &M;

			auto V = Mref.Data();
			auto& Vref = Mref.Data();
			const auto& V2ref = sib::MakeMatrix<short>(3, 3, 3).Data();
			auto V3 = sib::MakeMatrix<short>(3, 3, 3).Data();
			auto V4 = std::move(M).Data();
			decltype(auto) V5 = std::move(sib::MakeMatrix<short>(3, 3, 3).Data());
			decltype(auto) V6 = std::move(M).Data();

			decltype(auto) asdfas = std::move(int(4));
		}{
			auto M = sib::MakeMatrix<bool>(2, 4, 6, 2);
		}{
			auto DP = sib::TDimParam<unsigned, 4>(2, 4, 6, 2);
		}{
			int i;
			auto M = sib::TMatrixND<int, 3>{ 2, 3, i };
			auto DP = sib::TDimParam<unsigned, 3>{ 2, 3, i };
		}
	}
	catch (const std::exception& exc) {
		std::cerr << sib::ExceptionInfo(exc) << std::endl;
	}
	
	{
		try {
			int qqq = -10;
			auto hfg = sib::integral::cast<unsigned short>(qqq);
		}
		catch (const std::exception& exc) {
			std::cerr << sib::ExceptionInfo(exc) << std::endl;
		}
	}
	{
		try {
			int qqq = 1000000000;
			auto hfg = sib::integral::cast<unsigned short>(qqq);
		}
		catch (const std::exception& exc) {
			std::cerr << sib::ExceptionInfo(exc) << std::endl;
		}
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
