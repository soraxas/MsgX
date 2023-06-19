#include <doctest/doctest.h>

#include <Eigen/Dense>

#include "msgx/conversion/eigen.h"
#include "msgx/message.h"

using namespace msgx;
using Eigen::MatrixXd;

TEST_CASE("Test eigen")
{
    capnp::MallocMessageBuilder msg_builder;
    OpaqueMapping mapping{[&msg_builder]() { return msg_builder.getOrphanage(); }};

    MatrixXd m(6, 3);
    m << 1, 2, 3, 4, 5, 6,       //
        7, 8, 9, 10, 11, 12,     //
        13, 14, 15, 16, 17, 18;  //

    mapping["matrix"] = m;

    OpaqueItemBuilder builder = msg_builder.initRoot<msgx::type::Item::Oneof>();

    auto &my_matrix = mapping.get("matrix");
    CHECK(my_matrix);
    my_matrix->build(builder);

    CHECK_EQ(builder.which(), msgx::type::Item::Oneof::Which::ND_ARRAY);

    auto ndarray = builder.getNdArray();
    auto shape = ndarray.getMultiDimensional().getShape();
    CHECK_EQ(shape[0], 6);
    CHECK_EQ(shape[1], 3);

    CHECK_EQ(ndarray.getDtype(), msgx::type::ndarray::DataType::FLOAT64);
}
