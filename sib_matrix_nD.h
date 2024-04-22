#pragma once

#include <initializer_list>
#include <concepts>
#include <vector>
#include <array>

#include "sib_utilities.h"
#include "sib_exception.h"

namespace sib {

    class EMatrix;
    class EMatrixConstruct;

    using dim_t = unsigned char;

    // nD MATRIX **********************************************************************************************************
    template <typename type_, class alloc_ = std::allocator<type_>>
    class TMatrix {
    public:

        using TData = std::vector<type_, alloc_>;
        using size_type = typename TData::size_type;
            static_assert(size_type(-1) > size_type(0));
        using TSizes = std::vector<size_type>;

        using reference       = TData::reference      ;
        using const_reference = TData::const_reference;

    private:

        TSizes sizes;
        TData data;

    protected:

        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) < std::numeric_limits<dim_t>::max())
        constexpr TSizes InitSizes(const size_types_&... sizes)
        {
            try {
                return TSizes{ integral::cast<size_type>(sizes)... };
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Error initializing matrix size."));
            }
        }

        template <std::integral size_type_>
        constexpr TSizes InitSizes(size_type_* arr, size_t size_arr)
        {
            try {
                auto dimension_ = integral::cast<dim_t>(size_arr);
                auto tmp = TSizes(dimension_);
                for (size_t i = 0; i < dimension_; ++i) {
                    tmp[i] = integral::cast<size_type>(arr[i]);
                }
                return tmp;
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Error initializing matrix size."));
            }
        }

        constexpr TData InitData()
        {
            try {
                size_type total = 1;
                for (auto& s : sizes) { total = trydo::Multiply<size_type, size_type, size_type>(total, s); }
                return TData(total);
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Error initializing matrix."));
            }
        }

        size_type CalcTotalSize() noexcept
        {
            size_type total = 1;
            for (auto& s : sizes) { total *= s; }
            return total;
        }

    public:

        constexpr TMatrix() noexcept
            : sizes{}
            , data(1)
        {}

        template <std::integral ...size_types_>
        constexpr TMatrix(const size_types_&... sizes)
            : sizes(InitSizes(sizes...))
            , data(InitData())
        {}

        template <std::integral size_type_>
        constexpr TMatrix(const size_type_* arr, size_t size_arr)
            : sizes(InitSizes(arr, size_arr))
            , data(InitData())
        {}

        template <std::integral size_type_, dim_t dimension_>
        constexpr TMatrix(const size_type_(&arr)[dimension_])
            : sizes(InitSizes(&arr[0], dimension_))
            , data(InitData())
        {}

        constexpr dim_t Dimension() const noexcept(noexcept(TSizes.size())) { return sizes.size(); }

        constexpr size_type TotalDataSize() const noexcept(noexcept(TData.size())) { return data.size(); }

        const TData& Data() const noexcept { return data; }

        reference operator[] (const size_type pos) noexcept(noexcept(std::declval<TData>()[pos])) {
            return data[pos];
        }

        const_reference operator[](const size_type pos) const noexcept(noexcept(std::declval<const TData>()[pos])) {
            return data[pos];
        }

    };

    // SPECIFIC EXCEPTIONS **********************************************************************************************************

    class EMatrix : public std::runtime_error {
    public:
        using TBase = std::runtime_error;
        explicit EMatrix(std::string const& message) noexcept : TBase(message.c_str()) {}
        explicit EMatrix(char        const* message) noexcept : TBase(message        ) {}
    };

    class EMatrixConstruct : public EMatrix {
    public:
        using TBase = EMatrix;
        explicit EMatrixConstruct(std::string const& message) noexcept : TBase(message.c_str()) {}
        explicit EMatrixConstruct(char        const* message) noexcept : TBase(message        ) {}
    };

}
