#pragma once

#include <limits>
#include <format>
#include <stdexcept>

namespace sib {

    namespace integral {

#ifdef SIB_DEBUG_INTEGRAL_CAST_FULL
#define SIB_DEBUG_INTEGRAL_CAST_SIGN
#define SIB_DEBUG_INTEGRAL_CAST_OUT
#endif

#if defined(SIB_DEBUG_INTEGRAL_CAST_SIGN) ||        \
    defined(SIB_DEBUG_INTEGRAL_CAST_OUT)
#define SIB_DEBUG_INTEGRAL_CAST_ANY
#endif

#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_sign() noexcept {
            return std::signed_integral<SourceType> and std::unsigned_integral<TargetType>;
        }

        static_assert(__may_be_hit_by_sign<char  , short  >() == false);
        static_assert(__may_be_hit_by_sign<int   , size_t >() == false);
        static_assert(__may_be_hit_by_sign<size_t, int    >() == true );
        static_assert(__may_be_hit_by_sign<size_t, size_t >() == false);
        static_assert(__may_be_hit_by_sign<size_t, uint8_t>() == false);
#else
        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_sign() noexcept { return false; }
#endif

        template <std::integral TargetType, std::integral... SourceTypes>
            requires (sizeof...(SourceTypes) > 1)
        constexpr bool __may_be_hit_by_sign() noexcept {
            return (__may_be_hit_by_sign<TargetType, SourceTypes>() and ...);
        }

#ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_out() noexcept {
            using CommonType = std::common_type_t<SourceType, TargetType>;
            constexpr CommonType SourceMAX = (std::numeric_limits<SourceType>::max)();
            constexpr CommonType SourceMIN = (std::numeric_limits<SourceType>::min)();
            constexpr CommonType TargetMAX = (std::numeric_limits<TargetType>::max)();
            constexpr CommonType TargetMIN = (std::numeric_limits<TargetType>::min)();

            return (SourceMAX > TargetMAX) or (SourceMIN < TargetMIN);
        }

        static_assert(__may_be_hit_by_out<char     ,char     >() == false);
        static_assert(__may_be_hit_by_out<size_t   ,short    >() == false);
        static_assert(__may_be_hit_by_out<char     ,short    >() == true );
        static_assert(__may_be_hit_by_out<long     ,int      >() == false);
        static_assert(__may_be_hit_by_out<int      ,long long>() == true );
        static_assert(__may_be_hit_by_out<long long,unsigned >() == false);
        static_assert(__may_be_hit_by_out<long     ,unsigned >() == true );
#else
        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_out() noexcept { return false; }
#endif

        template <std::integral TargetType, std::integral... SourceTypes>
            requires (sizeof...(SourceTypes) > 1)
        constexpr bool __may_be_hit_by_out() noexcept {
            return (__may_be_hit_by_out<TargetType, SourceTypes>() and ...);
        }

        // INTEGRAL_CAST ===============================================================================================
        template <std::integral TargetType, std::integral SourceType>
        constexpr TargetType cast(const SourceType& val) 
#ifdef SIB_DEBUG_INTEGRAL_CAST_ANY
            noexcept (!__may_be_hit_by_sign<SourceType, TargetType>() and !__may_be_hit_by_out<SourceType, TargetType>())
#else
            noexcept(noexcept(static_cast<TargetType>(val)))
#endif
        {
#ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
            if constexpr (__may_be_hit_by_sign<SourceType, TargetType>()) {
                if (val < 0)
                    throw std::invalid_argument("Integral::cast negative to unsigned.");
            }
#endif
            auto result = static_cast<TargetType>(val);
#ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
            if constexpr (__may_be_hit_by_out<SourceType, TargetType>()) {
                if (val != result)
                    throw std::out_of_range("Out of range when integral::cast.");
            }
#endif
            return result;
        }

        struct MyStruct
        {

        };

    }

    namespace filter {

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
    
    }
        
    namespace trydo {
        
        template <typename TArg1, typename TArg2, typename TRes = decltype(std::declval<TArg1>() * std::declval<TArg2>())>
        constexpr TRes Multiply(const TArg1& arg1, const TArg2& arg2) {
            TRes(&_MAX_)(void) = std::numeric_limits<TRes>::max;
            if (arg2 == 0 or (_MAX_() / arg2 > arg1)) return arg1 * arg2;
            throw std::overflow_error(std::format("Overflow result in Multiply({}, {}).", arg1, arg2));
        }

    }
}
