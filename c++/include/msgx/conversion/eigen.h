#pragma once

#include "msgx/conversion/array.h"
#include "msgx/conversion/convertible.h"

// #include <Eigen/Dense>
// forward declare
namespace Eigen
{
template <typename Scalar, int, int, int, int, int>
class Matrix;
}

namespace msgx
{

template <typename Scalar, int... Options>
struct conversion<Eigen::Matrix<Scalar, Options...>>

{
    static void convert(OpaqueItemBuilder builder, const Eigen::Matrix<Scalar, Options...> &matrix)
    {
        SPDLOG_DEBUG("[Conversion] eigen matrix type {} with size '{}'", EASYSPDLOG_TYPE_NAME(Scalar), matrix.size());

        // thee following works for converting MatrixBase to Ref type to retrieve data()
        //    typedef Eigen::Ref<
        //        const Eigen::Matrix<typename Derived::Scalar, Derived::RowsAtCompileTime, Derived::ColsAtCompileTime,
        //        Derived::IsRowMajor>> RefB;
        //    RefB matrix_ref(matrix);
        // actual_b.data();

        auto ndarray_builder = builder.initNdArray();
        detail::numeric_ptr_to_ndarray<Scalar>(ndarray_builder, matrix.data(), matrix.size());

        auto dim = ndarray_builder.initMultiDimensional();
        dim.setColumnMajor(!matrix.IsRowMajor);
        dim.setShape({static_cast<uint32_t>(matrix.rows()), static_cast<uint32_t>(matrix.cols())});
    }
};

}  // namespace msgx
