//#define SIB_DEBUG_INTEGRAL_CAST_FULL
#define SIB_DEBUG_INTEGRAL_CAST_SIGN
#define SIB_DEBUG_INTEGRAL_CAST_OUT

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <vector>
#include <string>
#include <array>
#include <initializer_list>
#include <functional>
#include <unordered_map>

#include "sib_matrix_nD.h"
#include "sib_exception.h"
#include "sib_checks.h"

struct tB {};

struct tD : tB {
	char ch;
};
/*
constexpr bool sc_d_i = noexcept(static_cast<int   >(std::declval<double>()));
constexpr bool sc_i_d = noexcept(static_cast<double>(std::declval<int   >()));

constexpr bool sc_n_i = noexcept(std::enable_if_t<std::is_same_v< decltype( static_cast<int      >(std::declval<nullptr_t>()) ), int       >, std::true_type>::value);
constexpr bool sc_i_n = noexcept(std::enable_if_t<std::is_same_v< decltype( static_cast<nullptr_t>(std::declval<int      >()) ), nullptr_t >, std::true_type>::value);

constexpr bool sc_B_D = noexcept(static_cast<tD>(std::declval<tB>()));
constexpr bool sc_D_B = noexcept(static_cast<tB>(std::declval<tD>()));

constexpr bool dc_B_D = noexcept(std::enable_if_t<std::is_same_v< decltype( dynamic_cast<tD*>(std::declval<tB*>()) ), tD* >, std::true_type>::value);
constexpr bool dc_D_B = noexcept(std::enable_if_t<std::is_same_v< decltype( dynamic_cast<tB*>(std::declval<tD*>()) ), tB* >, std::true_type>::value);

constexpr bool rc_B_D = noexcept(reinterpret_cast<tD*>(std::declval<tB*>()));
constexpr bool rc_D_B = noexcept(reinterpret_cast<tB*>(std::declval<tD*>()));
*/

int main()
{
	try {

		int i = rand() * 10000;
		size_t st = sib::chk::integral_cast<unsigned char>(i);
		std::cout << st << std::endl;
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