#pragma once

#include <concepts>
#include <limits>
#include <format>

namespace sib {

    namespace integral {

        #ifdef SIB_DEBUG_INTEGRAL_CAST_FULL
            #define SIB_DEBUG_INTEGRAL_CAST_SIGN
            #define SIB_DEBUG_INTEGRAL_CAST_OUT
        #endif

        #if defined(SIB_DEBUG_INTEGRAL_CAST_SIGN) || \
            defined(SIB_DEBUG_INTEGRAL_CAST_OUT)
            #define SIB_DEBUG_INTEGRAL_CAST_ANY
        #endif

        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_sign()
            noexcept
        {
            #ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
                return std::signed_integral<SourceType> and std::unsigned_integral<TargetType>;
            #else
                return false;
            #endif
        }

        #ifdef SIB_DEBUG_INTEGRAL_CAST_SIGN
            static_assert(__may_be_hit_by_sign<int      , long long>() == false);
            static_assert(__may_be_hit_by_sign<long long, unsigned >() == false);
            static_assert(__may_be_hit_by_sign<unsigned , int      >() == true );
            static_assert(__may_be_hit_by_sign<unsigned , unsigned >() == false);
        #endif

        template <std::integral TargetType, std::integral... SourceTypes>
            requires (sizeof...(SourceTypes) > 1)
        constexpr bool __may_be_hit_by_sign()
            noexcept
        {
            return (__may_be_hit_by_sign<TargetType, SourceTypes>() and ...);
        }

        template <std::integral TargetType, std::integral SourceType>
        constexpr bool __may_be_hit_by_out()
            noexcept
        {
            #ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
                using CommonType = std::common_type_t<SourceType, TargetType>;
                constexpr CommonType SourceMAX = (std::numeric_limits<SourceType>::max)();
                constexpr CommonType SourceMIN = (std::numeric_limits<SourceType>::min)();
                constexpr CommonType TargetMAX = (std::numeric_limits<TargetType>::max)();
                constexpr CommonType TargetMIN = (std::numeric_limits<TargetType>::min)();
                return (SourceMAX > TargetMAX) or (SourceMIN < TargetMIN);
            #else
                return false;
            #endif
        }

        #ifdef SIB_DEBUG_INTEGRAL_CAST_OUT
            static_assert(__may_be_hit_by_out<char     ,char     >() == false);
            static_assert(__may_be_hit_by_out<long long,short    >() == false);
            static_assert(__may_be_hit_by_out<char     ,short    >() == true );
            static_assert(__may_be_hit_by_out<long     ,int      >() == false);
            static_assert(__may_be_hit_by_out<int      ,long long>() == true );
            static_assert(__may_be_hit_by_out<long long,unsigned >() == false);
            static_assert(__may_be_hit_by_out<long     ,unsigned >() == true );
        #endif

        template <std::integral TargetType, std::integral... SourceTypes>
            requires (sizeof...(SourceTypes) > 1)
        constexpr bool __may_be_hit_by_out()
            noexcept
        {
            return (__may_be_hit_by_out<TargetType, SourceTypes>() and ...);
        }

        // INTEGRAL_CAST ===============================================================================================
        template <std::integral TargetType, std::integral SourceType>
        constexpr TargetType cast(const SourceType& val) 
            noexcept (
                !__may_be_hit_by_sign<TargetType, SourceType>()
                and
                !__may_be_hit_by_out<TargetType, SourceType>()
                and
                noexcept(static_cast<TargetType>(val)))
        {
            if constexpr (__may_be_hit_by_sign<TargetType, SourceType>()) {
                if (val < 0)
                    throw std::invalid_argument("Integral::cast negative to unsigned.");
            }
            auto result = static_cast<TargetType>(val);
            if constexpr (__may_be_hit_by_out<TargetType, SourceType>()) {
                if (val != result)
                    throw std::out_of_range("Out of range when integral::cast.");
            }
            return result;
        }

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

        template <std::integral TargetType, std::integral SourceType>
        constexpr auto integral_cast(const SourceType& val) -> auto { return integral::cast<TargetType>(val); }

        template <typename TArg1, typename TArg2, typename TRes = decltype(std::declval<TArg1>() * std::declval<TArg2>())>
        constexpr TRes Multiply(const TArg1& arg1, const TArg2& arg2) {
            TRes(&_MAX_)(void) = std::numeric_limits<TRes>::max;
            if (arg2 == 0 or (_MAX_() / arg2 > arg1)) return arg1 * arg2;
            throw std::overflow_error(std::format("Overflow result in Multiply({}, {}).", arg1, arg2));
        }

    }
}
