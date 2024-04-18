#pragma once

#include <limits>
#include <format>
#include <stdexcept>

namespace sib {
    namespace chk {

#ifdef SIB_DEBUG_INTEGRAL_CAST_FULL
#define SIB_DEBUG_INTEGRAL_CAST_SIGN
#define SIB_DEBUG_INTEGRAL_CAST_OUT
#endif

#if defined(SIB_DEBUG_INTEGRAL_CAST_SIGN) ||        \
    defined(SIB_DEBUG_INTEGRAL_CAST_OUT)
#define SIB_DEBUG_INTEGRAL_CAST_ANY
#endif

#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
        template <std::integral SourceType, std::integral TargetType>
        constexpr bool __may_be_hit_by_sign() noexcept {
            return std::signed_integral<SourceType> and std::unsigned_integral<TargetType>;
        }

        static_assert(__may_be_hit_by_sign<short  , char  >() == false);
        static_assert(__may_be_hit_by_sign<size_t , int   >() == false);
        static_assert(__may_be_hit_by_sign<int    , size_t>() == true );
        static_assert(__may_be_hit_by_sign<size_t , size_t>() == false);
        static_assert(__may_be_hit_by_sign<uint8_t, size_t>() == false);
#endif

#ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
        template <std::integral SourceType, std::integral TargetType>
        constexpr bool __may_be_hit_by_out() noexcept {
            using CommonType = std::common_type_t<SourceType, TargetType>;
            constexpr CommonType SourceMAX = (std::numeric_limits<SourceType>::max)();
            constexpr CommonType SourceMIN = (std::numeric_limits<SourceType>::min)();
            constexpr CommonType TargetMAX = (std::numeric_limits<TargetType>::max)();
            constexpr CommonType TargetMIN = (std::numeric_limits<TargetType>::min)();

            return (SourceMAX > TargetMAX) or (SourceMIN < TargetMIN);
        }

        static_assert(__may_be_hit_by_out<char     , char     >() == false);
        static_assert(__may_be_hit_by_out<short    , size_t   >() == false);
        static_assert(__may_be_hit_by_out<short    , char     >() == true );
        static_assert(__may_be_hit_by_out<int      , long     >() == false);
        static_assert(__may_be_hit_by_out<long long, int      >() == true );
        static_assert(__may_be_hit_by_out<unsigned , long long>() == false);
        static_assert(__may_be_hit_by_out<unsigned , long     >() == true );
#endif

        // INTEGRAL_CAST ===============================================================================================
        template <std::integral TargetType, std::integral SourceType>
        constexpr TargetType integral_cast(const SourceType& val)
#ifdef SIB_DEBUG_INTEGRAL_CAST_ANY
            noexcept (!__may_be_hit_by_sign<SourceType, TargetType>() and !__may_be_hit_by_out<SourceType, TargetType>())
#else
            noexcept(noexcept(static_cast<TargetType>(val)))
#endif
        {
#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
            if constexpr (__may_be_hit_by_sign<SourceType, TargetType>()) {
                if (val < 0)
                    throw std::invalid_argument ("Integral_cast negative to unsigned.");
            }
#endif
            auto result = static_cast<TargetType>(val);
#ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
            if constexpr (__may_be_hit_by_out<SourceType, TargetType>()) {
                if (val != result)
                    throw std::out_of_range("Out of range when integral_cast.");
            }
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
