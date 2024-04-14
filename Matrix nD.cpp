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

template <std::integral F, std::integral... R>
struct integer_promotion_h {
	using type = decltype(F() + integer_promotion_h<R...>::type());
};

template <std::integral F, std::integral S>
struct integer_promotion_h<F, S> {
	using type = decltype(F() + S());;
};

template <std::integral T>
struct integer_promotion_h<T, T> {
	using type = T;
};

template <std::integral... Ts>
using integer_promotion_t = typename integer_promotion_h<Ts...>::type;

template <typename T>
concept unsigned_integral = std::integral<T> and std::is_unsigned_v<T>;

constexpr bool ItsLike(bool val) { return val; }

template <size_t N, unsigned_integral T>
class TMy {
public:
	using TData = T[N];
private:
    TData data;
public:
	template <std::integral... Ts>
		requires (sizeof...(Ts) == N)
	constexpr TMy(const Ts&... val)
		: data{ static_cast<T>(val)... }
	{
		static_assert(ItsLike(sib::chk::ThereNegative(val...) == 0), "Negative argument.");
	}
};

constexpr auto fgas = ItsLike(sib::chk::ThereNegative(2, 4lu, char(-5), 0, 3) == 0);

constexpr auto sdf = sib::chk::ThereNegative(2, 4lu, char(5), 0, 3);

std::common_type_t<int, size_t, unsigned char, unsigned long> nfy;

template <std::integral... Ts>
TMy(Ts...) -> TMy<sizeof...(Ts), std::common_type_t<Ts...>>;

constexpr TMy v1{ 2, 4lu, char(5), 3 };

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

		//auto sdf = std::vector{ 3, 5u, 6 };
		/*
		auto sdf = { 3, size_t(5), 6 };
		auto mds = sib::TMultiDimParam{ 3, size_t(5), 6 };

		sib::TMultiDimParam mds0(arr0);
		sib::TMultiDimParam mds1(arr1);
		
		sib::TMultiDimParam mds2(mds0);
		sib::TMultiDimParam mds3(mds1);

		sib::TMultiDimParam<5> mds4(ptr0);
		sib::TMultiDimParam<4> mds5(ptr1);
		*/

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