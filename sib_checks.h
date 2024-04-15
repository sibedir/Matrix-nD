#pragma once

#include <limits>
#include <format>
#include <stdexcept>
#include <variant>

#ifdef SIB_DEBUG_INTEGRAL_CAST_FULL
#define SIB_DEBUG_INTEGRAL_CAST_SIGN
#define SIB_DEBUG_INTEGRAL_CAST_OVERFLOW
#endif

namespace sib {
    namespace chk {

#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
        constexpr bool SIB_DEBUG_INTEGRAL_CAST_SIGN_ON = true;
#else
        constexpr bool SIB_DEBUG_INTEGRAL_CAST_SIGN_ON = false;
#endif
#ifdef SIB_DEBUG_INTEGRAL_CAST_OVERFLOW
        constexpr bool SIB_DEBUG_INTEGRAL_CAST_OVERFLOW_ON = true;
#else
        constexpr bool SIB_DEBUG_INTEGRAL_CAST_OVERFLOW_ON = false;
#endif

        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __hit_with_sign(SourceType) { return false; }

        template <std::unsigned_integral TargetType, std::integral SourceType>
        constexpr bool __hit_with_sign(SourceType source) { return source < 0; }

        template <std::integral TargetType, std::integral SourceType>
        constexpr TargetType integral_cast(const SourceType& val)
            noexcept(!SIB_DEBUG_INTEGRAL_CAST_SIGN_ON and !SIB_DEBUG_INTEGRAL_CAST_OVERFLOW_ON)
        {
#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
            if (__hit_with_sign<TargetType>(val))
                throw std::underflow_error("Negative to unsigned.");
#endif
            auto result = static_cast<TargetType>(val);
#ifdef SIB_DEBUG_INTEGRAL_CAST_OVERFLOW
            if (result != val)
                throw std::overflow_error("Overflow in integral_cast.");
#endif
            return result;
        }

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

        template <typename TRes, typename TArg1, typename TArg2>
        constexpr TRes TryMultiply(const TArg1& arg1, const TArg2& arg2) {
            static constexpr auto MAX = std::numeric_limits<TRes>::max();
            if (arg2 == 0 or (MAX / arg2 > arg1)) return arg1 * arg2;
            throw std::overflow_error(std::format("Overflow result in MultiplyTry({}, {}).", arg1, arg2));
        }

        template <typename T>
        constexpr T TryMultiply(const T& arg1, const T& arg2) {
            return MultiplyTry<T, T, T>(arg1, arg2);
        }
    }
}
