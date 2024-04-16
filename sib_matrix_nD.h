#pragma once

#include <initializer_list>
#include <concepts>
#include <vector>
#include <array>

#include "sib_checks.h"
#include "sib_exception.h"

namespace sib {

    class EMatrix;
    class EMatrixConstruct;
    
    // HOLELINE ***********************************************************************************************************
    template <typename Type>
    class THoleLine {
    public:

        using difference_type = decltype(std::declval<Type*>() - std::declval<Type*>());
        using size_type = std::make_unsigned_t<difference_type>;

    private:

        Type* space;

    protected:

    public:

    };

    // nD MATRIX **********************************************************************************************************
    using dim_t = unsigned char;

    template <dim_t Dimension, typename Type, class Alloc = std::allocator<Type>>
    class TMatrix {
    public:

        using TData = std::vector<Type, Alloc>;
        using size_type = typename TData::size_type;
        using TSizeArr = size_type[Dimension];

        using reference       = TData::reference      ;
        using const_reference = TData::const_reference;

    private:

        TSizeArr size;
        TSizeArr capacity;
        TData data;

    protected:

        size_type CalcTotalCapacity() {
            auto res = size_type(1);
            for (auto& s : capacity) { res *= s; }
            return res;
        }

    public:

        TMatrix() noexcept
            : size{}
            , capacity{}
            , data(0)
        {}

        template <std::convertible_to<size_type> ...SizeTypes>
            requires (sizeof...(SizeTypes) == Dimension)
        TMatrix(const SizeTypes... sizes)
            : size{ static_cast<size_type>(sizes)... }
            , capacity{ static_cast<size_type>(sizes)... }
            , data(CalcTotalCapacity)
        {
        }

        template <std::integral size_type>
        TMatrix(const size_type(&arr)[Dimension])
            : size()
            , data()
        {}

        template <std::integral size_type>
        TMatrix(const size_type* arr, dim_t size_arr = Dimension)
            : size()
            , data()
        {}

        //constexpr dimension_type Dimension() const noexcept { return Dim; }

        size_type DataSize() const noexcept(noexcept(data.size())) { return data.size(); }

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

    class EMatrixSizes : public EMatrix {
    public:
        using TBase = EMatrix;
        explicit EMatrixSizes(std::string const& message) noexcept : TBase(message.c_str()) {}
        explicit EMatrixSizes(char        const* message) noexcept : TBase(message        ) {}
    };

    class EMatrixSizesConstruct : public EMatrixSizes {
    public:
        using TBase = EMatrixSizes;
        explicit EMatrixSizesConstruct(std::string const& message) noexcept : TBase(message.c_str()) {}
        explicit EMatrixSizesConstruct(char        const* message) noexcept : TBase(message        ) {}
    };
}
