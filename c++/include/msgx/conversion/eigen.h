#pragma once

#include <Eigen/Dense>

#include "msgx/conversion/array.h"

namespace msgx
{

namespace conversion
{

template <typename Scalar, int... Options>
void opaque_item(msgx::BindableOpaqueItem &opaque_item, const Eigen::Matrix<Scalar, Options...> &matrix)
{
    SPDLOG_DEBUG("[Conversion] eigen matrix type {} with size '{}'", typeid(DataType).name(), matrix.size());

    // using Container = Eigen::Matrix<Scalar, Options...>;
    if (!opaque_item.has_orphanage())
        throw std::runtime_error("Eigen matrix opaque item only support ptr with orphanage");

    auto builder = opaque_item.get_orphan_or_malloc_builder();

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

}  // namespace conversion
}  // namespace msgx
