#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>

#include "sib_matrix_nD.h"
#include "sib_exception.h"

/*
template<typename T, unsigned char Size>
struct TMy {
	std::vector<T> data = std::vector<T>(Size);

	template <typename ...size_types>
	TMy(const size_types... sizes) {}
};

template<typename T>
struct TMy<T, 0> {
	T data;
	TMy() {}
};
*/

struct TMyStruct {
	int value;
	TMyStruct() = delete;
//	TMtStruct(int value) : value(value) {};
};
using Tarr = int[3];
using Tarr_ptr = int(*)[3];

int main()
{
	

	int dsa[3]{ 1, 2, 3 };
	auto klna = new int[3] {1, 2, 3};
	std::remove_reference_t<decltype(std::declval<int[3]>())>* arr = &dsa;
	int(&& arr23)[3] = std::move(dsa);





	std::array<int, 3> ms_arr{ 8, 4, 3 };

	std::array<int, 15> arr1 = { 5, 4, 3, 2, 1 };
	std::array<int, 15> arr2;

	arr2 = arr1;
	std::cout << sizeof(arr1) << std::endl;

	std::vector<int> bv;
	bv.resize(20);

	decltype(auto) v1 = bv[1];
	auto& v2 = bv[2];

	v1 = 1;

	auto it = bv.begin();

	try {
		sib::TMatrix<3, int> matrix0(100, 100, 100);// , 435, 3465, 56435, 6543, 2345, 1234);
		//matrix.Resize(4, 0, 700);// , 435, 3465, 56435, 6543, 2345, 1234);
		std::cout << matrix0.DataSize() << std::endl;
		auto& data = matrix0.Data();
		auto dim  = matrix0.Dimension;
		auto size = matrix0.DataSize();

		matrix0[10] = 1;

		decltype(auto) val = matrix0[7];
		val = 1;

		sib::TMatrix<1, int> matrix2(30000);


		/*double*/ int arr[] = {40, 50, 70};
		static constexpr auto aaa = sizeof(arr) / sizeof(int);
		static constexpr auto bbb = sib::TMatrix<3, int>::Dimension;
		std::cout << aaa << "   " << int(bbb) << std::endl;
		//static_assert((sizeof(arr) / sizeof(int)) == 3, "***********");
		sib::TMatrix<3, int> matrix3(arr);
		std::cout << matrix3.DataSize() << std::endl;


		std::vector vec1 { 40, 50, 70};
		//std::vector<size_t> vec2 = static_cast<std::vector<size_t>>(vec1);
		sib::TMatrix<3, int> matrix4(vec1.data());
		std::cout << matrix4.DataSize() << std::endl;

	}
	catch (const std::exception& exc) {
		std::cerr << sib::ExceptionInfo(exc) << std::endl;
	}

//THE_END:
	std::cout << std::endl << std::endl;
	std::cout << "  - THE END -  " << std::endl;
	std::cout << "press [Enter]...";
	std::cin.get();
	exit(0);
}