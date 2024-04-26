#pragma once

#include <initializer_list>
#include <type_traits>
#include <concepts>
#include <vector>
#include <array>

#include "sib_utilities.h"

namespace sib {

    #ifdef SIB_DEBUG_MATRIX_FULL
        #define SIB_DEBUG_DIMENSION_TOTAL_OVERFLOW
    #endif

    constexpr bool __may_be_hit_by_dim_total_overflow()
        noexcept
    {
        #ifdef SIB_DEBUG_DIMENSION_TOTAL_OVERFLOW
            return true;
        #else
            return false;
        #endif
    }

    // DIMENSION PARAM ****************************************************************************************************
    using dim_t = unsigned char;

    template <std::unsigned_integral type_, dim_t dimension_>
    class TDimParam {
    public:

        static constexpr dim_t dimension = dimension_;
        using value_type = type_;
        using TData = std::array<value_type, dimension>;

    private:

        template <std::integral arg_type_, size_t... idx_>
        [[nodiscard]] constexpr TData InitData(arg_type_* arr, std::index_sequence<idx_...>)
        {
            return { integral::cast<value_type>(arr[idx_])... };
        }

    protected:

        TData mydata;

        template <std::integral arg_type_>
        [[nodiscard]] constexpr TData InitData(arg_type_* arr)
        {
            return InitData(arr, std::make_index_sequence<dimension>{});
        }

        template <std::integral arg_type_>
        [[nodiscard]] constexpr TData InitData(const arg_type_* arr, size_t arr_size)
        {
            if (arr_size < dimension) throw std::length_error("Error initializing dimension parameters. Initialization buffer is too small.");
            return InitData(arr, std::make_index_sequence<dimension>{});
        }

    public:

        constexpr TDimParam() noexcept
            : mydata{}
        {}

        template <std::integral ...args_types_>
            requires (sizeof...(args_types_) == dimension)
        constexpr explicit TDimParam(const args_types_&... args)
            : mydata{ integral::cast<value_type>(args)... }
        {}

        template <std::integral arg_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TDimParam(const arg_type_(&arr)[arr_size_])
            : mydata(InitData(&arr[0]))
        {}

        template <std::integral arg_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TDimParam(const std::array<arg_type_, arr_size_>& arr)
            : mydata(InitData(&arr[0]))
        {}

    protected:

        constexpr value_type Total()
            noexcept(!__may_be_hit_by_dim_total_overflow())
        {
            value_type total = 1;
            for (auto& s : mydata) {
                if constexpr (__may_be_hit_by_dim_total_overflow()) {
                    total = trydo::Multiply<value_type, value_type, value_type>(total, s);
                }
                else {
                    total *= s;
                }
            }
            return total;
        }

    };

    // MATRIX INTERFACE ***************************************************************************************************
    template <typename type_, typename alloc_ = std::allocator<type_>>
    class TMatrix {
    public:

        using TData           = std::vector<type_, alloc_>;
        using value_type      = typename TData::value_type     ;
        using size_type       = typename TData::size_type      ;  static_assert(size_type(-1) > size_type(0)); // I'm watching you (-_-)
        using reference       = typename TData::reference      ;
        using const_reference = typename TData::const_reference;

    protected:

        TData mydata;

        template <typename... Ts>
        constexpr TMatrix(Ts&&... args) : mydata(args...) {};

    public:

        constexpr const TData&  Data() const &  noexcept { return           mydata ; }
        constexpr       TData&& Data()       && noexcept { return std::move(mydata); }

    };

    // MATRIX nD **********************************************************************************************************
    template <typename type_, dim_t dimension_, typename alloc_ = std::allocator<type_>>
    class TMatrixND 
        : public TDimParam<typename TMatrix<type_, alloc_>::size_type, dimension_>
        , public TMatrix<type_, alloc_> {
    public:

        using TMatrix = TMatrix<type_, alloc_>;
        using typename TMatrix::TData          ;
        using typename TMatrix::value_type     ;
        using typename TMatrix::size_type      ;
        using typename TMatrix::reference      ;
        using typename TMatrix::const_reference;

        static constexpr dim_t dimension = dimension_;
        using TSizes = TDimParam<size_type, dimension>;
    public:

        constexpr TMatrixND() noexcept
            : TSizes()
            , TMatrix(1)
        {}

        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) == dimension)
        constexpr TMatrixND(const size_types_&... sizes)
            : TSizes{ integral::cast<size_type>(sizes)... }
            , TMatrix(TSizes::Total())
        {}

        template <std::integral size_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMatrixND(const size_type_(&arr)[arr_size_])
            : TSizes(arr)
            , TMatrix(TSizes::Total())
        {}

        template <std::integral size_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMatrixND(const std::array<size_type_, arr_size_>& arr)
            : TSizes(arr)
            , TMatrix(TSizes::Total())
        {}

    };

    template <typename type_, std::integral ...size_types_, typename alloc_ = std::allocator<type_>>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    TMatrixND(size_types_...) -> TMatrixND<type_, sizeof...(size_types_), alloc_>;

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral ...size_types_>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    [[nodiscard]] constexpr auto MakeMatrix(const size_types_&... sizes)
    {
        return TMatrixND<type_, sizeof...(size_types_), alloc_>(sizes...);
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, dim_t dimension_>
    [[nodiscard]] constexpr auto MakeMatrix(const size_type_(&arr)[dimension_])
    {
        return TMatrixND<type_, dimension_, alloc_>(arr);
    }

    template <typename type_, dim_t dimension_, typename alloc_ = std::allocator<type_>, std::integral size_type_, size_t arr_size_>
        requires (dimension_ <= arr_size_)
    [[nodiscard]] constexpr auto MakeMatrix(const size_type_(&arr)[arr_size_])
    {
        return TMatrixND<type_, dimension_, alloc_>(arr);
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, dim_t dimension_>
    [[nodiscard]] constexpr auto MakeMatrix(const std::array<size_type_, dimension_>& arr)
    {
        return TMatrixND<type_, dimension_, alloc_>(arr);
    }

    template <dim_t dimension_, typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, size_t arr_size_>
        requires (dimension_ <= arr_size_)
    [[nodiscard]] constexpr auto MakeMatrix(const std::array<size_type_, arr_size_>& arr)
    {
        return TMatrixND<type_, dimension_, alloc_>(arr);
    }

}
