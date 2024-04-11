#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>

#include "sib_matrix_nD.h"
#include "sib_exception.h"

template <typename T>
struct MyStruct {
	T data;

	MyStruct(int value) noexcept(false)
		: data(value)
	{}

	operator T() noexcept {
		return data;
	}
};

template <typename ...T>
void foo (const T... sizes) noexcept(noexcept(static_cast<size_t>(T()))) {}


int main()
{
	std::vector<int> sdfg;
	sdfg.resize(213);

	try {
		std::array<int, 3>;

		int arr[5] { 1, 9, 4, 3, 5 };

		sib::TMultiDimParam<5> mds0(arr);

		int d = 8;

		constexpr auto b0 = noexcept(static_cast<MyStruct<size_t>>(1));
		constexpr auto b1 = noexcept(foo(MyStruct<size_t>(1), 2, 3));
		constexpr auto b2 = noexcept(foo(1, 2, 3, 4, 5));

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