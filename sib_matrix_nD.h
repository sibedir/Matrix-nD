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

    // nD MATRIX SIZES ****************************************************************************************************

    template <std::integral T>
    struct as_unsigned_h { using type = T; };

    template <> struct as_unsigned_h<bool     > { using type = bool              ; };
    template <> struct as_unsigned_h<char     > { using type = unsigned char     ; };
    template <> struct as_unsigned_h<short    > { using type = unsigned short    ; };
    template <> struct as_unsigned_h<int      > { using type = unsigned int      ; };
    template <> struct as_unsigned_h<long     > { using type = unsigned long     ; };
    template <> struct as_unsigned_h<long long> { using type = unsigned long long; };

    template <std::integral T>
    using as_unsigned = typename as_unsigned_h<T>::type;

    using dimension_t = unsigned char;

    template <dimension_t Dimension_, std::integral SizeType__>
    class TMultiDimParam;

    template<typename First_, typename... Rest_>
    TMultiDimParam(First_, Rest_...) ->
        TMultiDimParam<
            sizeof...(Rest_) + 1,
            std::enable_if_t<
                std::conjunction_v< std::is_same<First_, Rest_>... >,
                First_
            >
        >;

    template <dimension_t Dimension_, std::integral SizeType__ = size_t>
    class TMultiDimParam {
    public:
        
        using SizeType = as_unsigned<SizeType__>;
        using TData = SizeType[Dimension_];

    private:

        TData data;

    public:

        TMultiDimParam() = delete;

        template <std::integral ...size_types>
            requires (sizeof...(size_types) == Dimension_)
        constexpr explicit TMultiDimParam(const size_types... sizes)
            noexcept(noexcept(static_cast<SizeType>(size_types())))
            : data{ static_cast<SizeType>(sizes)... }
        {}

        constexpr TMultiDimParam(std::initializer_list<SizeType> initlist)
            noexcept
            : data{ initlist }
        {}

        template <std::integral size_type>
            requires (!std::is_same_v<size_type, SizeType>)
        constexpr explicit TMultiDimParam(std::initializer_list<size_type> initlist)
            noexcept(noexcept(static_cast<SizeType>(size_type())))
            : data{ static_cast<SizeType>(initlist)... }
        {}

        constexpr explicit TMultiDimParam(const SizeType* arr)
        {
            std::memcpy(data, arr, sizeof(SizeType) * Dimension_);
        }

        template <chk::integral_pointer size_type_ptr>
            requires (!std::is_same_v<size_type_ptr, SizeType*>)
        constexpr explicit TMultiDimParam(size_type_ptr source, const dimension_t length = Dimension_)
        {
            std::cout << "--------------------" << std::endl;
            std::cout << typeid(size_type_ptr).name() << std::endl;
            std::cout << typeid(SizeType*).name() << std::endl;
            for (dimension_t i = 0; i < Dimension_; ++i) {
                data[i] = static_cast<SizeType>(source[i]);
            }
        }

        template <dimension_t dimension_, std::integral size_type>
        constexpr explicit TMultiDimParam(const size_type(&)[dimension_]) = delete;

        template <dimension_t dimension_, std::integral size_type>
            requires (dimension_ == Dimension_)
        constexpr explicit TMultiDimParam(const size_type(&arr)[dimension_])
            : TMultiDimParam(&arr[0])
        {}

        template <typename T>
        constexpr explicit TMultiDimParam(const TMultiDimParam<Dimension_, T>& other)
            : TMultiDimParam(other.Data())
        {}

        constexpr const TData& Data() const { return data; };
        constexpr       TData& Data()       { return data; };

        constexpr const SizeType& operator[](const dimension_t pos) const
            noexcept(noexcept(data[pos]))
        {
            return data[pos];
        }

        constexpr       SizeType& operator[](const dimension_t pos)
            noexcept(noexcept(data[pos]))
        {
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
