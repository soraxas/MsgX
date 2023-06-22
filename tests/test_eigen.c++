#include <Eigen/Dense>

#include "test_common.h"

using Eigen::MatrixXd;

TEST_CASE("Test eigen")
{
    MessageX msg;

    std::unique_ptr<OpaqueMapping> mapping = msg.getLinkedItemPtr<OpaqueMapping>();

    MatrixXd m(6, 3);
    m << 1, 2, 3, 4, 5, 6,       //
        7, 8, 9, 10, 11, 12,     //
        13, 14, 15, 16, 17, 18;  //

    (*mapping)["matrix"] = m;

    OpaqueItemBuilder builder = get_oneof_builder(msg);

    auto &my_matrix = mapping->get("matrix");
    CHECK(my_matrix);
    my_matrix->build(builder);

    CHECK_EQ(builder.which(), Which::ND_ARRAY);

    auto ndarray = builder.getNdArray();
    auto shape = ndarray.getMultiDimensional().getShape();
    CHECK_EQ(shape[0], 6);
    CHECK_EQ(shape[1], 3);

    CHECK_EQ(ndarray.getDtype(), msgx::type::ndarray::DataType::FLOAT64);
}
