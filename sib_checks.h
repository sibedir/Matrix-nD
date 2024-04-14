#pragma once

#include <limits>
#include <format>
#include <stdexcept>

namespace sib {
    namespace chk {
        template <typename T>
        using Clean = std::remove_cvref_t<T>;

        template <typename T>
        concept integral_pointer =
            !std::is_array_v<Clean<T>>
            and
            std::is_pointer_v<Clean<T>>
            and
            std::integral<std::remove_pointer_t<Clean<T>>>;

        template <typename T>
        concept integral_array =
            std::is_array_v<Clean<T>>
            and
            std::integral<std::remove_extent_t<Clean<T>>>;

        template <typename T, size_t N>
        concept integral_array_N =
            std::is_array_v<Clean<T>>
            and
            std::is_same_v<Clean<T>, std::remove_extent_t<Clean<T>>[N]>
            and
            std::integral<std::remove_extent_t<Clean<T>>>;

        template <typename type>
        constexpr type ThereNegative(const type& arg) {
            if (arg < 0) return arg;
            return 0;
        }

        template <typename type, typename ...types>
        constexpr std::common_type_t<type, types...> ThereNegative(const type& first_arg, const types&... args) {
            if (first_arg < 0) return first_arg;
            return ThereNegative(args...);
        }

        template <typename type, size_t size>
        constexpr type ThereNegative(const type(&arr)[size]) {
            for (const auto& val : arr) {
                if (val < 0) return val;
            }
            return 0;
        }

        template <typename type, typename size_type>
        constexpr type ThereNegative(const type* const arr, size_type size) {
            for (size_type idx = 0; idx < size; ++idx) {
                if (arr[idx] < 0) return arr[idx];
            }
            return 0;
        }

        template <typename type, class Alloc = std::allocator<type>>
        constexpr type ThereNegative(const std::vector<type, Alloc>& vec) {
            for (const auto& val : vec) {
                if (val < 0) return val;
            }
            return 0;
        }

        template <typename ...types>
        constexpr void TryArePositive(types... args) {
            auto test = ThereNegative(args...);
            if (test)
                throw std::invalid_argument(std::format("Negative argument: {}.", test));
        }


        template <typename TRes, typename TArg1, typename TArg2>
        constexpr TRes MultiplyTry(const TArg1& arg1, const TArg2& arg2) {
            static constexpr auto MAX = std::numeric_limits<TRes>::max();
            if (arg2 == 0 or (MAX / arg2 > arg1)) return arg1 * arg2;
            throw std::overflow_error(std::format("Overflow result in MultiplyTry({}, {}).", arg1, arg2));
        }

        template <typename T>
        constexpr T MultiplyTry(const T& arg1, const T& arg2) {
            return MultiplyTry<T, T, T>(arg1, arg2);
        }
    }
}
