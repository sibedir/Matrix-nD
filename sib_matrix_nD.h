#pragma once

#include <initializer_list>
#include <concepts>
#include <vector>
#include <array>

#include "sib_utilities.h"
#include "sib_exception.h"

namespace sib {

#ifdef SIB_DEBUG_MATRIX_FULL
#define SIB_DEBUG_MATRIX_SIZE_OVERFLOW
#endif

    class EMatrix;
    class EMatrixConstruct;

    using dim_t = unsigned char;

    // nD MATRIX **********************************************************************************************************
    template <dim_t dimension_, typename type_, typename alloc_ = std::allocator<type_>>
    class TMatrix {
    public:

        static constexpr dim_t dimension = dimension_;
        using value_type = type_;
        using TData = std::vector<type_, alloc_>;
        using size_type = typename TData::size_type;
            static_assert(size_type(-1) > size_type(0)); // I'm watching you (-_-)
        using TSizes = std::array<size_type, dimension_>;

        using reference       = TData::reference      ;
        using const_reference = TData::const_reference;

    private:

        TSizes mysizes;
        TData mydata;

    protected:

        size_type CalcTotalSize() noexcept
        {
            size_type total = 1;
            for (auto& s : mysizes) { total *= s; }
            return total;
        }

    public:

        constexpr TMatrix() noexcept
            : mysizes{}
            , mydata(1)
        {}

        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
        constexpr TMatrix(const size_types_&... sizes)
        {
            try {
                mysizes = TSizes{ integral::cast<size_type>(sizes)... };
            }
            catch (...) { std::throw_with_nested(EMatrixConstruct("Error initializing matrix size.")); }
#ifdef SIB_DEBUG_MATRIX_SIZE_OVERFLOW
            try {
                size_type total = 1;
                for (auto& s : mysizes) { total = trydo::Multiply<size_type, size_type, size_type>(total, s); }
                mydata.resize(total);
            }
            catch (...) { std::throw_with_nested(EMatrixConstruct("Error initializing matrix.")); }
#else
            mydata.resize(CalcTotalSize());
#endif
        }

        template <std::integral size_type_>
        constexpr TMatrix(const size_type_* arr, size_t arr_size)
        {
            try {
                for (size_t i = 0; i < dimension; ++i) {
                    mysizes[i] = integral::cast<size_type>(arr[i]);
                }
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Error initializing matrix size."));
            }
#ifdef SIB_DEBUG_MATRIX_SIZE_OVERFLOW
            try {
                size_type total = 1;
                for (auto& s : mysizes) { total = trydo::Multiply<size_type, size_type, size_type>(total, s); }
                mydata.resize(total);
            }
            catch (...) { std::throw_with_nested(EMatrixConstruct("Error initializing matrix.")); }
#else
            mydata.resize(CalcTotalSize());
#endif
        }

        template <dim_t dimension__ = dimension, std::integral size_type_>
        constexpr TMatrix(const size_type_(&arr)[dimension__])
            : TMatrix::TMatrix(&arr[0])
        {};

        constexpr size_type TotalDataSize() const
            noexcept(noexcept(mydata.size()))
        {
            return mydata.size();
        }

        const TData& Data() const
            noexcept
        {
            return mydata;
        }

        reference operator[](const size_type pos)
            noexcept(noexcept(std::declval<TData>()[pos]))
        {
            return mydata[pos];
        }

        const_reference operator[](const size_type pos) const
            noexcept(noexcept(std::declval<const TData>()[pos]))
        {
            return mydata[pos];
        }

        reference at(const size_type(&arr)[])
        {
        }

        template <std::integral... size_types_>
        reference operator()(size_types_... pos)
        {
            size_type pos_arr[]{ integral::cast<size_type>(pos)... };
        }

    };

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral ...size_types_>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    constexpr auto MakeMatrix(const size_types_&... sizes)
    {
        return TMatrix<sizeof...(size_types_), type_, alloc_>(sizes...);
    }

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
