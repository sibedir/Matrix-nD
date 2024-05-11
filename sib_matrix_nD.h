#pragma once

#include <initializer_list>
#include <type_traits>
#include <concepts>
#include <vector>
#include <array>

#include "sib_utilities.h"

namespace sib {

    using dim_t = unsigned char;

    template <dim_t dimension_, std::unsigned_integral type_> class TMultiDimParam;
    template <typename type_, dim_t dimension_, typename alloc_> class TMatrixND;

    #ifdef SIB_DEBUG_MATRIX_FULL
        #define SIB_DEBUG_DIMENSION_TOTAL_OVERFLOW
        #define SIB_DEBUG_DIMENSION_DYNAMIC_CHECK
    #endif

    constexpr bool __may_be_hit_by_dim_total_overflow =
        #ifdef SIB_DEBUG_DIMENSION_TOTAL_OVERFLOW
            true;
        #else
            false;
        #endif
    
    constexpr bool __may_be_hit_by_dim_dyn_check =
        #ifdef SIB_DEBUG_DIMENSION_DYNAMIC_CHECK
            true;
        #else
            false;
        #endif
    
    // DIMENSION PARAMETERS INTERFACE ******************************************************************************************
    template <std::unsigned_integral type_>
    class IMultiDimParam {
    public:

        using value_type = type_;

        virtual ~IMultiDimParam() = default;

    private:

        template <std::integral arg_type_, dim_t dimension_>
        [[nodiscard]] static constexpr auto Make(const std::vector<arg_type_>& vec)
        {
            return std::unique_ptr<IMultiDimParam> { new TMultiDimParam<dimension_, arg_type_>(vec) };
        }

        template <std::integral arg_type_, dim_t... dim_pack_>
        [[nodiscard]] static constexpr auto InitMakersArray(std::integer_sequence<dim_t, dim_pack_...>)
        {
            return std::array{ &Make<arg_type_, dim_pack_>..., &Make<arg_type_, std::numeric_limits<dim_t>::max()> };
        }

        template <std::integral arg_type_>
        static constexpr auto MakersArray =
            InitMakersArray<arg_type_>(std::make_integer_sequence<dim_t, std::numeric_limits<dim_t>::max()>());

    public:

        template <std::integral arg_type_>
        [[nodiscard]] static auto New(const std::vector<arg_type_>& vec)
        {
            return (*MakersArray<arg_type_>[vec.size()])(vec);
        }

        template <std::integral arg_type_>
        [[nodiscard]] static auto New(const std::vector<arg_type_>& vec, const dim_t dimension)
        {
            return (*MakersArray<arg_type_>[dimension])(vec);
        }

    protected:

        virtual constexpr value_type Total() const noexcept(!__may_be_hit_by_dim_total_overflow) = 0;
        
    public:

        virtual constexpr dim_t Dimension() const noexcept = 0;

        virtual constexpr value_type operator[] (dim_t index) const = 0;

    };
    
    template <typename left_type_, typename right_type_>
    constexpr bool operator== (const IMultiDimParam<left_type_>& left, const IMultiDimParam<right_type_>& right)
    {
        if (left.Dimension() != right.Dimension())
            return false;

        for (size_t i = 0; i < left.Dimension(); ++i) {
            if (left[i] != right[i]) return false;
        }
        return true;
    }
    
    // DIMENSION PARAMETERS ****************************************************************************************************
    template <dim_t dimension_, std::unsigned_integral type_>
    class TMultiDimParam
        : public IMultiDimParam<type_>{
    public:

        using IMultiDimParam = IMultiDimParam<type_>;
        static constexpr dim_t dimension = dimension_;
        using value_type = type_;
        using TData = std::array<value_type, dimension>;

    protected:

        TData mydata;

        template <std::integral arg_type_, size_t... idx_>
            requires (sizeof...(idx_) == dimension)
        [[nodiscard]] constexpr TData InitData(arg_type_* arr, std::index_sequence<idx_...>)
        {
            return { integral::cast<value_type>(arr[idx_])... };
        }

        template <std::integral arg_type_>
        [[nodiscard]] constexpr TData InitData(arg_type_* arr)
        {
            return InitData(arr, std::make_index_sequence<dimension>{});
        }

        template <std::integral arg_type_>
        [[nodiscard]] constexpr TData InitData(const arg_type_* arr, size_t arr_size)
        {
            if constexpr (__may_be_hit_by_dim_dyn_check) {
                if (arr_size < dimension)
                    throw std::length_error("Error initializing dimension parameters. Initialization buffer is too small.");
            }
            return InitData(arr, std::make_index_sequence<dimension>{});
        }

    public:

        constexpr TMultiDimParam() noexcept
            : mydata{}
        {}

        template <std::integral ...args_type_>
            requires (sizeof...(args_type_) == dimension)
        constexpr TMultiDimParam(const args_type_&... args)
            : mydata{ integral::cast<value_type>(args)... }
        {}
        
        constexpr TMultiDimParam(const type_(&arr)[dimension_])
            : mydata(InitData(&arr[0]))
        {}

        template <std::integral arg_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMultiDimParam(const arg_type_(&arr)[arr_size_])
            : mydata(InitData(&arr[0]))
        {}

        constexpr TMultiDimParam(const std::array<type_, dimension_>& arr)
            : mydata(InitData(&arr[0]))
        {}

        template <std::integral arg_type_, size_t arr_size_>
            requires(dimension <= arr_size_)
        constexpr TMultiDimParam(const std::array<arg_type_, arr_size_>& arr)
            : mydata(InitData(&arr[0]))
        {}

        template <std::integral arg_type_>
        constexpr TMultiDimParam(const std::vector<arg_type_>& vec)
            : mydata(InitData(vec.data(), vec.size()))
        {}

        virtual ~TMultiDimParam() = default;

    public:

        constexpr dim_t Dimension() const noexcept override
        {
            return dimension;
        }

    protected:

        constexpr value_type Total() const noexcept(!__may_be_hit_by_dim_total_overflow) override
        {
            value_type total = 1;
            for (auto& s : mydata) {
                if constexpr (__may_be_hit_by_dim_total_overflow) {
                    total = trydo::Multiply<value_type, value_type, value_type>(total, s);
                }
                else {
                    total *= s;
                }
            }
            return total;
        }

    public:

        constexpr TData const & Data() const & noexcept { return           mydata ; }
        constexpr TData      && Data()      && noexcept { return std::move(mydata); }

        constexpr value_type operator[] (dim_t index) const override
        {
            return mydata[index];
        };

    };

    template <dim_t left_dim_, typename left_type_, dim_t right_dim_, typename right_type_>
    constexpr bool operator== (const TMultiDimParam<left_dim_, left_type_>& left, const TMultiDimParam<right_dim_, right_type_>& right)
    {
        if (left.dimension != right.dimension)
            return false;

        for (size_t i = 0; i < left.dimension; ++i) {
            if (left[i] != right[i]) return false;
        }
        return true;
    }

    template <typename seed_type_, dim_t dimension_, typename alloc_ = std::allocator<seed_type_>>
        requires (std::is_same_v<seed_type_, typename alloc_::value_type>)
    using TDimParam—onductor = TMultiDimParam<dimension_, typename alloc_::size_type>;

    template <std::integral... args_type_>
        requires (sizeof...(args_type_) <= std::numeric_limits<dim_t>::max())
    [[nodiscard]] constexpr auto MakeMultiDimParam(const args_type_&... args)
    {
        return TMultiDimParam<sizeof...(args_type_), std::make_unsigned_t<std::common_type_t<args_type_...>>>(args...);
    }

    template <dim_t dimension_, std::integral arg_type_>
    [[nodiscard]] constexpr auto MakeMultiDimParam(const arg_type_(&arr)[dimension_])
    {
        return TMultiDimParam<dimension_, std::make_unsigned_t<arg_type_>>(arr);
    }

    template <std::integral arg_type_, size_t arr_size_>
    [[nodiscard]] constexpr auto MakeMultiDimParam(const std::array<arg_type_, arr_size_>& arr)
    {
        return TMultiDimParam<arr_size_, std::make_unsigned_t<arg_type_>>(arr);
    }

    template <dim_t dimension_, std::unsigned_integral type_, std::integral arg_type_>
    [[nodiscard]] constexpr auto MakeMultiDimParam(const std::vector<arg_type_>& vec)
    {
        return TMultiDimParam<dimension_, type_>(vec);
    }
    
    template <dim_t dimension_, std::integral arg_type_>
    [[nodiscard]] constexpr auto MakeMultiDimParam(const std::vector<arg_type_>& vec)
    {
        return TMultiDimParam<dimension_, std::make_unsigned_t<arg_type_>>(vec);
    }

    // MATRIX INTERFACE ***************************************************************************************************
    template <typename type_, typename alloc_ = std::allocator<type_>>
    class IMatrix {
    public:

        using TData           = std::vector<type_, alloc_>;
        using value_type      = typename TData::value_type     ;
        using size_type       = typename TData::size_type      ;  static_assert(size_type(-1) > size_type(0)); // I'm watching you (-_-)
        using reference       = typename TData::reference      ;
        using const_reference = typename TData::const_reference;

    protected:

        TData mydata;

        template <typename... Ts>
        constexpr IMatrix(Ts&&... args) : mydata(args...) {};

    private:

        template <std::integral arg_type_, dim_t dimension_>
        [[nodiscard]] static constexpr auto Make(const std::vector<arg_type_>& vec)
        {
            return std::unique_ptr<IMatrix> { new TMatrixND<type_, dimension_, alloc_>(vec) };
        }

        template <std::integral arg_type_, dim_t... dim_pack_>
        [[nodiscard]] static constexpr auto InitMakersArray(std::integer_sequence<dim_t, dim_pack_...>)
        {
            return std::array{ &Make<arg_type_, dim_pack_>..., &Make<arg_type_, std::numeric_limits<dim_t>::max()> };
        }

        template <std::integral arg_type_>
        static constexpr auto MakersArray =
            InitMakersArray<arg_type_>(std::make_integer_sequence<dim_t, std::numeric_limits<dim_t>::max()>());

    public:

        template <std::integral arg_type_>
        [[nodiscard]] static auto New(const std::vector<arg_type_>& vec)
        {
            return (*MakersArray<arg_type_>[vec.size()])(vec);
        }

        template <std::integral arg_type_>
        [[nodiscard]] static auto New(const std::vector<arg_type_>& vec, const dim_t dimension)
        {
            return (*MakersArray<arg_type_>[dimension])(vec);
        }

        virtual ~IMatrix() = default;

        virtual constexpr dim_t Dimension() const noexcept = 0;

        virtual IMultiDimParam<size_type> const & Sizes() const noexcept = 0;

        constexpr TData const & Data() const & noexcept { return           mydata ; }
        constexpr TData      && Data()      && noexcept { return std::move(mydata); }

    };

    // MATRIX nD **********************************************************************************************************
    template <typename type_, dim_t dimension_, typename alloc_ = std::allocator<type_>>
    class TMatrixND final
        : public TMultiDimParam<dimension_, typename IMatrix<type_, alloc_>::size_type>
        , public IMatrix<type_, alloc_> {
    public:

        using TMatrix = IMatrix<type_, alloc_>;
        using typename TMatrix::TData          ;
        using typename TMatrix::value_type     ;
        using typename TMatrix::size_type      ;
        using typename TMatrix::reference      ;
        using typename TMatrix::const_reference;

        static constexpr dim_t dimension = dimension_;
        using TSizes = TMultiDimParam<dimension, size_type>;

    public:

        constexpr TMatrixND() noexcept
            : TSizes()
            , TMatrix(1)
        {}
        
        template <std::integral ...size_types_>
            requires (sizeof...(size_types_) == dimension)
        constexpr TMatrixND(const size_types_&... sizes)
            : TSizes(sizes...)
            , TMatrix(TSizes::Total())
        {}
        
        constexpr TMatrixND(const TSizes& dimparam)
            : TSizes(dimparam)
            , TMatrix(TSizes::Total())
        {}
        

        virtual ~TMatrixND() = default;

    public:
        
        constexpr dim_t Dimension() const noexcept override
        {
            return dimension;
        }

        IMultiDimParam<size_type> const & Sizes() const noexcept override
        {
            return *this;
        }

        inline constexpr TData const & Data() const & noexcept { return             TMatrix         ::Data(); }
        inline constexpr TData      && Data()      && noexcept { return static_cast<TMatrix&&>(*this).Data(); }

    };
    
    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral ...size_types_>
        requires (sizeof...(size_types_) <= std::numeric_limits<dim_t>::max())
    [[nodiscard]] constexpr auto MakeMatrix(const size_types_&... sizes)
    {
        return TMatrixND<type_, sizeof...(size_types_), alloc_>({ sizes... });
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
    
    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral arg_type_>
    [[nodiscard]] constexpr auto MakeMatrix(const std::vector<arg_type_>& vec)
    {
        return IMatrix<type_, alloc_>::New(vec);
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral arg_type_>
    [[nodiscard]] constexpr auto MakeMatrix(const std::vector<arg_type_>& vec, const dim_t dimension)
    {
        return IMatrix<type_, alloc_>::New(vec, dimension);
    }
        
    template <typename type_, typename alloc_ = std::allocator<type_>, typename size_type_ = alloc_::size_type, dim_t dimension_>
    [[nodiscard]] constexpr auto MakeMatrix(TMultiDimParam<dimension_, size_type_>&& dimparam)
    {
        return TMatrixND<type_, dimension_, alloc_>(dimparam);
    }
    
}
