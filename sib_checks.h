#pragma once

#include <limits>
#include <format>
#include <stdexcept>

namespace sib {
    namespace chk {
        template <typename T>
        using ñlean = std::remove_cvref_t<T>;

        template <typename T, bool arr = std::is_array_v<T>>
        concept _integral_pointer_if_arr = !arr and std::is_pointer_v<ñlean<T>> and std::integral<std::remove_pointer_t<ñlean<T>>>;

        template <typename T>
        concept integral_pointer = _integral_pointer_if_arr<T>;

        template <typename T>
        concept integral_array = std::is_array_v<ñlean<T>> and std::integral<std::remove_extent_t<ñlean<T>>>;

        template <typename T, size_t N>
        concept integral_array_N =
            std::is_array_v<ñlean<T>>
            and
            std::is_same_v<ñlean<T>, std::remove_extent_t<ñlean<T>>[N]>
            and
            std::integral<std::remove_extent_t<ñlean<T>>>;

        template <typename type>
        inline void AssertArePositive(type arg) {
            if (arg < 0) throw std::invalid_argument(std::format("Negative argument: {}.", arg));
        }

        template <typename type, typename ...types>
        inline void AssertArePositive(type first_arg, types... args) {
            if (first_arg < 0)
                throw std::invalid_argument(std::format("Negative argument: {}.", first_arg));
            AssertArePositive(args...);
        }

        template <typename type, size_t size>
        inline void AssertArePositive(const type(&arr)[size]) {
            for (const auto& val : arr) {
                AssertArePositive<type>(val);
            }
        }

        template <typename type, typename size_type>
        inline void AssertArePositive(const type* const arr, size_type size) {
            for (size_type idx = 0; idx < size; ++idx) {
                AssertArePositive<type>(arr[idx]);
            }
        }

        template <typename type, class Alloc = std::allocator<type>>
        inline void AssertArePositive(const std::vector<type, Alloc>& vec) {
            for (const auto& val : vec) {
                AssertArePositive<type>(val);
            }
        }

        template <typename TRes, typename TArg1, typename TArg2>
        inline TRes MultiplyTry(const TArg1& arg1, const TArg2& arg2) {
            static constexpr auto MAX = std::numeric_limits<TRes>::max();
            if (arg2 == 0 or (MAX / arg2 > arg1)) return arg1 * arg2;
            throw std::overflow_error(std::format("Overflow result in MultiplyTry({}, {}).", arg1, arg2));
        }

        template <typename T>
        inline T MultiplyTry(const T& arg1, const T& arg2) {
            return MultiplyTry<T, T, T>(arg1, arg2);
        }
    }
}