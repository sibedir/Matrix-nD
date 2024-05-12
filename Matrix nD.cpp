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
			auto DP2 = sib::MakeMultiDimParam(1, 2, 4, 6, 2, 2);
			auto DP1 = sib::TMultiDimParam<6, size_t>(1, 2, 4, 6, 2, 2);

			auto M1 = sib::MakeMatrix<short>(1, 2, 4, 6, 2, 2);
			auto M2 = sib::TMatrixND<short, 6>(1, 2, 4, 6, 2, 2);
			auto& M3 = M1;
			auto M4 = M1;
			auto M5 = std::move(M1);
	
		}{
			constexpr int arr[]{ 3, 5, 1, 6, 5 };
			auto DP1 = sib::MakeMultiDimParam(arr);
			auto DP2 = sib::TMultiDimParam<5, unsigned>(arr);
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			constexpr size_t arr[]{ 3, 5, 1, 6, 5 };
			auto DP1 = sib::MakeMultiDimParam(arr);
			auto DP2 = sib::TMultiDimParam(arr);
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			constexpr std::array arr{ 1, 2, 3, 2, 1 };
			auto DP1 = sib::MakeMultiDimParam(arr);
			auto DP2 = sib::TMultiDimParam<5, unsigned>(arr);
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			constexpr std::array<size_t, 5> arr{ 1, 2, 3, 2, 1 };
			auto DP1 = sib::MakeMultiDimParam(arr);
			auto DP2 = sib::TMultiDimParam<5, unsigned>(arr);
			auto M1 = sib::MakeMatrix<int>(arr);
			auto M2 = sib::TMatrixND<int, 5>(arr);
		}{
			constexpr size_t arr1[]{ 2, 3, 4, 5 };
			sib::TMultiDimParam DP1 = arr1;

			constexpr size_t arr2[]{ 2, 3, 4, 6 };
			auto DP2 = sib::TMultiDimParam<4, unsigned>(arr2);

			constexpr unsigned char arr3[]{ 2, 3, 4, 5 };
			sib::TMultiDimParam DP3 = arr3;

			auto b1 = DP1 == DP2;
			auto b2 = DP1 == DP3;
			auto b3 = DP2 == DP3;

			auto M1 = sib::TMatrixND<int, 4>(arr1);
			auto M2 = sib::TMatrixND<int, 4>(DP1);
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
			auto s0 = QQQ();
			auto s1 = sib::TMatrixND<int, 3>(4, 5, 3).Data().size();
			constexpr int arr[]{ 3, 5, 2 };
			auto s2 = std::move(sib::MakeMatrix<int>(arr)).Data().size();
		}{
			auto M = sib::MakeMatrix<short>(1, 2, 4, 6, 2, 2);
			sib::IMatrix<short>& Mref = M;
			sib::IMatrix<short>* Mptr = &M;

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
			auto DP = sib::TMultiDimParam<4, unsigned>(2, 4, 6, 2);
		}{
			int i = 4;
			//auto M = sib::TMatrixND<int, 3>{ 2, -3, i };
			//auto DP = sib::TDimParam<unsigned, 3>{ 2, -3, i };
			auto M = sib::TMatrixND<int, 3>( 2, 3, i );
			auto DP = sib::TMultiDimParam<3, unsigned>( 2, 3, i );
		}/*{
			auto vec1 = std::vector{ 2, 3, 4 };
			auto DP0 = sib::MakeMultiDimParam<3>(vec1);
			auto DP1 = sib::TMultiDimParam<3, unsigned>(vec1);
			auto DP2 = sib::MakeMultiDimParam<3>(vec1);
			auto M1 = sib::TMatrixND<unsigned, 3>(vec1);
			auto M2 = sib::MakeMatrix<unsigned>(vec1);
			auto vec2 = std::vector{ 2, 3, 4 };
			auto DP3 = sib::TMultiDimParam<3, unsigned char>(vec2);
			auto DP4 = sib::MakeMultiDimParam<3, unsigned char>(vec2);
			auto M3 = sib::TMatrixND<char, 3>(vec1);
			auto M4 = sib::MakeMatrix<char>(vec1);
			auto M5 = sib::MakeMatrix<char>(vec1, 2);
			auto M111 = sib::MakeMatrix<float>(vec1);
			auto M222 = sib::MakeMatrix<double>(vec1);
		}*/{
			auto vec = std::vector{ 2, 3, 4, 7, 3, 3 };
			auto M = sib::MakeMatrix<int>(vec, 5);
			std::cout << M->Data().size() << std::endl;
			std::cout << static_cast<int>(M->Dimension()) << std::endl;
			for (size_t d = 0; d < M->Dimension(); ++d) {
				std::cout << M->Sizes()[d] << " ";
			}
			std::cout << std::endl;
		}{
			auto vec = std::vector{ 2, 3, 4, 7, 3, 3 };
			auto M = sib::MakeMultiDimParam(vec, 5);
			std::cout << M->Data().size() << std::endl;
			std::cout << static_cast<int>(M->Dimension()) << std::endl;
			for (size_t d = 0; d < M->Dimension(); ++d) {
				std::cout << M->Sizes()[d] << " ";
			}
			std::cout << std::endl;
		}
	}
	catch (const std::exception& exc) {
		std::cerr << sib::exception::Info(exc) << std::endl;
	}
	
	goto THE_END;

	{
		try {
			int qqq = -10;
			auto www = sib::integral::cast<unsigned short>(qqq);
			std::cout << www << std::endl;
		}
		catch (const std::exception& exc) {
			std::cerr << sib::exception::Info(exc) << std::endl;
		}
	}
	{
		try {
			int qqq = 1000000000;
			auto hfg = sib::integral::cast<unsigned short>(qqq);
		}
		catch (const std::exception& exc) {
			std::cerr << sib::exception::Info(exc) << std::endl;
		}
	}
	
THE_END:
	std::cout << std::endl << std::endl;
	std::cout << "  - THE END -  " << std::endl;
	std::cout << "press [Enter]...";
	std::cin.clear();
	//std::cin.get();
	std::cin.get();
	exit(0);
}
