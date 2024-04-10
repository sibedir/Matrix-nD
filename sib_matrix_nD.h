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

    template <dimension_t Dimension, std::unsigned_integral SizeType>
    class TMultiDimSize {
    public:

        //using size_type = SizeType;

    private:

        SizeType data[Dimension];

    protected:

        

    public:
        TMultiDimSize() = delete;

        template <std::integral ...size_types>
        TMultiDimSize(const size_types... sizes) :
            data(sizes...)
        {}

        template <std::integral size_type>
        TMultiDimSize(const size_type(&arr)[Dimension])
            : data(arr)
        {}

        template <std::integral_ size_type>
        TMultiDimSize(const size_type* arr, dimension_type size_arr = Dimension)
            : sizes(InitSizes(arr, size_arr))
            , data(InitData())
        {}
    };

    // nD MATRIX **********************************************************************************************************
    template <unsigned char Dimension, typename T, class Alloc = std::allocator<T>>
    class TMatrix {
    public:

        using dimension_type = unsigned char;
        static constexpr dimension_type Dimension = Dimension;
        using TData = std::vector<T, Alloc>;
        using data_size_type = typename TData::size_type;
        using TSizes = std::vector<data_size_type>;
        //using TSizes = data_size_type[Dimension];
        using sizes_size_type = typename TSizes::size_type;
        using reference = TData::reference;
        using const_reference = TData::const_reference;

    private:

        TSizes sizes;
        TData  data;

    protected:

        template <typename ...size_types, typename std::enable_if_t< std::conjunction_v<std::is_integral<size_types>...>, bool > = true>
        TSizes InitSizes(size_types... sizes) {
            static_assert(sizeof...(sizes) == Dimension, "Constructor arguments count is not equal matrix dimension.");

            TSizes tmp;
            try {
                tmp = { static_cast<data_size_type>(sizes)... };

                AssertArePositive(sizes...);

                return tmp;
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Matrix sizes initialization error."));
            }
        }

        template <std::integral size_type>
        TSizes InitSizes(const size_type(&arr)[Dimension]) {
            TSizes tmp;
            try {
                tmp.resize(Dimension);
                for (dimension_type i = 0; i < Dimension; ++i) {
                    AssertArePositive(arr[i]);
                    tmp[i] = static_cast<sizes_size_type>(arr[i]);
                }
                return tmp;
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Matrix sizes initialization error."));
            }
        }

        template <std::integral size_type>
        TSizes InitSizes(const size_type* arr, dimension_type size_arr) {
            TSizes tmp;
            try {
                tmp.resize(size_arr);
                for (dimension_type i = 0; i < size_arr; ++i) {
                    AssertArePositive(arr[i]);
                    tmp[i] = static_cast<sizes_size_type>(arr[i]);
                }
                if (size_arr != Dimension) {
                    throw std::invalid_argument(std::format("Initializing dynamic array size ({}) is not equal matrix dimension ({}).", size_arr, Dimension));
                }
                return tmp;
            }
            catch (...) {
                std::throw_with_nested(EMatrixConstruct("Matrix sizes initialization error."));
            }
        }

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
            : sizes(Dimension, 0)
            , data(0)
        {}

        template <typename ...size_types, typename std::enable_if_t< std::conjunction_v<std::is_integral<size_types>...>, bool > = true>
        TMatrix(const size_types... sizes)
            : sizes(InitSizes(sizes...))
            , data(InitData())
        {}

        template <std::integral size_type>
        TMatrix(const size_type(&arr)[Dimension])
            : sizes(InitSizes(arr))
            , data(InitData())
        {}

        template <std::integral size_type>
        TMatrix(const size_type* arr, dimension_type size_arr = Dimension)
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
