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

template <typename T>
concept _array = std::is_array_v<T>;

template <typename T>
concept _pointer = std::is_pointer_v<T> and !_array<T>;

template <_pointer T>
void foo(const T a) {
	std::cout << typeid(a).name() << std::endl;
}

template <_array T>
void foo(const T a) {
	std::cout << typeid(a).name() << std::endl;
}

template <typename T, size_t N>
void foo(const T(&a)[N]) {
	std::cout << typeid(a).name() << std::endl;
}

int main()
{
	using TArr = int[5];

	int i = 10;
	int* ptr = &i;
	TArr arr { 1, 9, 4, 3, 8 };

	constexpr auto is_ptr = _pointer<TArr>; // is_ptr = false
	constexpr auto is_arr = _array<TArr>;   // is_arr = true

	foo(ptr);
	foo(arr);

	try {
		int arr0[5]{ 1, 9, 4, 3, 8 };
		size_t arr1[4]{ 6, 2, 2, 9 };

		using arr_t = decltype(arr);
		constexpr auto b0 = std::is_array_v<arr_t>;
		constexpr auto b1 = sib::chk::integral_array<arr_t>;
		constexpr auto b2 = sib::chk::integral_array_N<arr_t, 5>;
		constexpr auto b3 = sib::chk::integral_pointer<arr_t>;

		//sib::TMultiDimParam<5, unsigned short> mds0(arr0);
		//sib::TMultiDimParam<5> mds1(std::move(arr1));
		//sib::TMultiDimParam<5> mds2(mds0);
		//sib::TMultiDimParam<5> mds3(mds1);

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