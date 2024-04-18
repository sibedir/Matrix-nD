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

    template <typename type_, dim_t dimension_, class alloc_ = std::allocator<type_>>
    class TMatrix {
    public:

        using TData = std::vector<type_, alloc_>;
        using size_type = typename TData::size_type;
        using TSize = std::vector<size_type>;

        using reference       = TData::reference      ;
        using const_reference = TData::const_reference;

    private:

        TSize size;
        TData data;

    protected:

        size_type CalcTotalSize() {
            auto res = size_type(1);
            for (auto& s : size) { res *= s; }
            return res;
        }

    public:

        constexpr TMatrix() noexcept
            : size{}
            , data(0)
        {}

        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) == dimension_)
        constexpr TMatrix(const size_types_&... sizes)
            : size{ chk::integral_cast<size_type>(sizes)... }
            , data(CalcTotalSize())
        {}

        constexpr TMatrix(const size_type(&arr)[dimension_])
            : size()
            , data()
        {}

        constexpr TMatrix(const size_type* arr, dim_t size_arr = dimension_)
            : size()
            , data()
        {}

        constexpr dim_t Dimension() const noexcept { return dimension_; }

        size_type DataSize() const noexcept(noexcept(data.size())) { return data.size(); }

        const TData& Data() const noexcept { return data; }

        reference operator[] (const size_type pos) noexcept(noexcept(std::declval<TData>()[pos])) {
            return data[pos];
        }

        const_reference operator[](const size_type pos) const noexcept(noexcept(std::declval<const TData>()[pos])) {
            return data[pos];
        }

    };

    template <typename type_, std::integral ...size_types_>
    TMatrix(size_types_...) -> TMatrix<type_, sizeof...(size_types_)>;

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
