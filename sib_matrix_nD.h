#pragma once

#include <initializer_list>
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

    template <dim_t dimension_, std::unsigned_integral type_>
    class TDimParam {
    public:

        static constexpr dim_t dimension = dimension_;
        using value_type = type_;
        using TData = std::array<value_type, dimension>;

    private:

        TData mydata;

        template <std::integral arg_type_, size_t... idx_>
        [[nodiscard]] constexpr TData InitData(arg_type_* arr, std::index_sequence<idx_...>)
        {
            return { integral::cast<value_type>(arr[idx_])... };
        }

    protected:

        constexpr value_type Total()
            noexcept(!__may_be_hit_by_dim_total_overflow())
        {
            value_type total = 1;
            for (auto& s : mysizes) {
                if constexpr (__may_be_hit_by_dim_total_overflow()) {
                    total = trydo::Multiply<size_type, size_type, size_type>(total, s);
                }
                else {
                    total *= s;
                }
            }
            return total;
        }

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
        constexpr TDimParam(const args_types_&... args)
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
            : mysizes(InitData(&arr[0]))
        {}

    };

    // MATRIX INTERFACE ***************************************************************************************************
    template <typename type_, typename alloc_ = std::allocator<type_>>
    class IMatrix {
    public:

        using TData           = std::vector<type_, alloc_>;
        using value_type      = typename TData::value_type     ;
        using size_type       = typename TData::size_type      ;  static_assert(size_type(-1) > size_type(0)); // I'm watching you (-_-)
        using reference       = typename TData::reference      ;
        using const_reference = typename TData::const_reference;

    public:

        virtual constexpr const TData&  Data() const &  = 0;
        virtual constexpr       TData&& Data()       && = 0;

    };

    // MATRIX nD **********************************************************************************************************
    template <dim_t dimension_, typename type_, typename alloc_ = std::allocator<type_>>
    class TMatrixND : public IMatrix<type_, alloc_> {
    public:

        using TBase = IMatrix<type_, alloc_>;
        using typename TBase::TData          ;
        using typename TBase::value_type     ;
        using typename TBase::size_type      ;
        using typename TBase::reference      ;
        using typename TBase::const_reference;

        static constexpr dim_t dimension = dimension_;
        using TSizes = std::array<size_type, dimension>;

    private:

        TSizes mysizes;
        TData mydata;

        template <std::integral size_type_, size_t... idx_>
        [[nodiscard]] constexpr TSizes InitSizes(size_type_* arr, std::index_sequence<idx_...>)
        {
            return { integral::cast<size_type>(arr[idx_])... };
        }

    protected:

        constexpr size_type TotalSize()
            #ifndef SIB_DEBUG_MATRIX_SIZE_OVERFLOW
            noexcept
            #endif
        {
            size_type total = 1;
            for (auto& s : mysizes) {
            #ifdef SIB_DEBUG_MATRIX_SIZE_OVERFLOW
                total = trydo::Multiply<size_type, size_type, size_type>(total, s);
            #else
                total *= s;
            #endif
            }
            return total;
        }

        template <std::integral size_type_>
        [[nodiscard]] constexpr TSizes InitSizes(size_type_* arr)
        {
            return InitSizes(arr, std::make_index_sequence<dimension>{});
        }

        template <std::integral size_type_>
        constexpr TMatrixND(const size_type_* arr, size_type arr_size)
        {
            for (size_t i = 0; i < dimension; ++i) {
                mysizes[i] = integral::cast<size_type>(arr[i]);
            }
            mydata.resize(TotalSize());
        }

    public:

        constexpr TMatrixND() noexcept
            : mysizes{}
            , mydata(1)
        {}

        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) == dimension)
        constexpr TMatrixND(const size_types_&... sizes)
            : mysizes{ integral::cast<size_type>(sizes)... }
            , mydata(TotalSize())
        {}

        template <std::integral size_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMatrixND(const size_type_(&arr)[arr_size_])
            : mysizes(InitSizes(&arr[0]))
            , mydata(TotalSize())
        {}

        template <std::integral size_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMatrixND(const std::array<size_type_, arr_size_>& arr)
            : mysizes(InitSizes(&arr[0]))
            , mydata(TotalSize())
        {}

        constexpr const TData&  Data() const &  noexcept final override { return           mydata ; }
        constexpr       TData&& Data()       && noexcept final override { return std::move(mydata); }

    };

    template <typename type_, std::integral ...size_types_, typename alloc_ = std::allocator<type_>>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    TMatrixND(size_types_...) -> TMatrixND<sizeof...(size_types_), type_, alloc_>;

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral ...size_types_>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    [[nodiscard]] constexpr auto MakeMatrix(const size_types_&... sizes)
    {
        return TMatrixND<sizeof...(size_types_), type_, alloc_>(sizes...);
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, dim_t dimension_>
    [[nodiscard]] constexpr auto MakeMatrix(const size_type_(&arr)[dimension_])
    {
        return TMatrixND<dimension_, type_, alloc_>(arr);
    }

    template <dim_t dimension_, typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, size_t arr_size_>
        requires (dimension_ <= arr_size_)
    [[nodiscard]] constexpr auto MakeMatrix(const size_type_(&arr)[arr_size_])
    {
        return TMatrixND<dimension_, type_, alloc_>(arr);
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, dim_t dimension_>
    [[nodiscard]] constexpr auto MakeMatrix(const std::array<size_type_, dimension_>& arr)
    {
        return TMatrixND<dimension_, type_, alloc_>(arr);
    }

    template <dim_t dimension_, typename type_, typename alloc_ = std::allocator<type_>, std::integral size_type_, size_t arr_size_>
        requires (dimension_ <= arr_size_)
    [[nodiscard]] constexpr auto MakeMatrix(const std::array<size_type_, arr_size_>& arr)
    {
        return TMatrixND<dimension_, type_, alloc_>(arr);
    }

}
