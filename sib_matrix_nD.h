#pragma once

#include <concepts>
#include <vector>
#include <array>

#include "sib_checks.h"
#include "sib_exception.h"

namespace sib {

    class EMatrix;
    class EMatrixConstruct;
    class EMatrixSizes;
    class EMatrixSizesConstruct;

    // nD MATRIX SIZES ****************************************************************************************************

    using dimension_t = unsigned char;


    template <dimension_t Dimension_, std::unsigned_integral SizeType_ = size_t>
    class TMultiDimParam {
    public:

        using TData = SizeType_[Dimension_];

    private:

        TData data;

    public:

        TMultiDimParam() = delete;

        template <std::integral ...size_types>
            requires (sizeof...(size_types) == Dimension_)
        TMultiDimParam(const size_types... sizes) noexcept(noexcept(static_cast<SizeType_>(size_types())))
            : data{ static_cast<SizeType_>(sizes)... }
        {}

        template <integral_pointer size_type_ptr>
        TMultiDimParam(const size_type_ptr arr, const dimension_t size_arr = Dimension_) {
            for (dimension_t i = 0; i < Dimension_; ++i) {
                data[i] = static_cast<SizeType_>(arr[i]);
            }
        }

        TMultiDimParam(const SizeType_* arr) {
            std::memcpy(data, arr, sizeof(SizeType_) * Dimension_);
        }

        template <dimension_t dimension_, std::integral size_type>
        TMultiDimParam(const size_type(&arr)[dimension_]) = delete;

        template <dimension_t dimension_, std::integral size_type>
            requires (dimension_ == Dimension_)
        TMultiDimParam(const size_type(&arr)[dimension_]) : TMultiDimParam(&arr[0]) {}

        template <typename T>
        TMultiDimParam(const TMultiDimParam<Dimension_, T>& other) : TMultiDimParam(other.Data()) {}

        TData& Data() {
            return data;
        };

        const TData& Data() const {
            return data;
        };

        constexpr SizeType_& operator[] (const dimension_t pos) noexcept(noexcept(data[pos])) {
            return data[pos];
        }

        constexpr const SizeType_& operator[](const dimension_t pos) const noexcept(noexcept(data[pos])) {
            return data[pos];
        }

    };

    // nD MATRIX **********************************************************************************************************
    template <dimension_t Dimension_, typename T, class Alloc = std::allocator<T>>
    class TMatrix {
    public:

        using TData = std::vector<T, Alloc>;
        using data_size_type = typename TData::size_type;
        using TSizes = TMultiDimParam<Dimension_, data_size_type>;

        using reference       = TData::reference      ;
        using const_reference = TData::const_reference;

    private:

        TSizes sizes;
        TData  data;

    protected:

        TData InitData() {
            try {
                data_size_type size = 1;
                for (auto& s : sizes)
                    size = MultiplyTry(size, s);

                return TData(size);
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Matrix data initialization error."));
            }
        }

    public:

        TMatrix() noexcept
            : sizes()
            , data(0)
        {}

        template <typename ...size_types, typename std::enable_if_t< std::conjunction_v<std::is_integral<size_types>...>, bool > = true>
        TMatrix(const size_types... sizes)
            : sizes(InitSizes(sizes...))
            , data(InitData())
        {}

        template <std::integral size_type>
        TMatrix(const size_type(&arr)[Dimension_])
            : sizes(InitSizes(arr))
            , data(InitData())
        {}

        template <std::integral size_type>
        TMatrix(const size_type* arr, dimension_t size_arr = Dimension_)
            : sizes(InitSizes(arr, size_arr))
            , data(InitData())
        {}

        //constexpr dimension_type Dimension() const noexcept { return Dim; }

        data_size_type DataSize() const noexcept(noexcept(data.size())) { return data.size(); }

        const TData& Data() const noexcept { return data; }

        reference operator[] (const data_size_type pos) noexcept(noexcept(std::declval<TData>()[pos])) {
            return data[pos];
        }

        const_reference operator[](const data_size_type pos) const noexcept(noexcept(std::declval<const TData>()[pos])) {
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
