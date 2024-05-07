#include "sib_matrix_nD.h"

namespace sib {

    using dim_t = unsigned char;

    template <typename type_, dim_t dimension_, typename alloc_ = std::allocator<type_>, std::integral arg_type_>
    [[nodiscard]] auto __MakeMatrixByVector(const std::vector<arg_type_>& vec)
    {
        return std::unique_ptr<TMatrix<type_, alloc_>> { new TMatrixND<type_, dimension_, alloc_>(vec) };
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral arg_type_, dim_t... dimensions_>
    [[nodiscard]] auto __InitMatrixMakersByVector(std::integer_sequence<dim_t, dimensions_...>)
    {
        return std::array{ &__MakeMatrixByVector<type_, dimensions_, alloc_, arg_type_>... };
    }

    template <typename type_, typename alloc_ = std::allocator<type_>, std::integral arg_type_ = typename alloc_::size_type>
    static std::array< TMakerMatrixByVector<type_, alloc_, arg_type_>, std::numeric_limits<dim_t>::max() > MatrixMakersByVector =
        __InitMatrixMakersByVector<type_, alloc_, arg_type_>(std::make_integer_sequence<dim_t, std::numeric_limits<dim_t>::max()>());

}