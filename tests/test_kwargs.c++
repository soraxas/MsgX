#include "test_common.h"

using namespace msgx;

#define DOCTEST_MSGX_PARAMETERIZED_KWARGS(VALUE, KWARGS_KEY, ONEOF_GETTER, VALUE_TYPE)                                 \
    /*    DOCTEST_SUBCASE((std::string("Test anylist storing type [" #VALUE_TYPE "]").c_str())) */                     \
    {                                                                                                                  \
        auto builder = get_oneof_builder(msg);                                                                         \
        auto raw_ptr = downcast_bindable_item(mapping->get(KWARGS_KEY).get());                                         \
                                                                                                                       \
        raw_ptr->build(builder);                                                                                       \
        CHECK_EQ(builder.which(), VALUE_TYPE);                                                                         \
        CHECK_EQ(builder.ONEOF_GETTER(), VALUE);                                                                       \
    }

TEST_CASE("Test kwargs primitive")
{
    using namespace msgx::kwargs;
    msgx::MessageX msg;

    auto mapping = msg.Mapping("int"_kw = 1, "double"_kw = 2., "string"_kw = "okk");

    DOCTEST_MSGX_PARAMETERIZED_KWARGS(1, "int", getInt, Item::Oneof::INT);
    DOCTEST_MSGX_PARAMETERIZED_KWARGS(2., "double", getDouble, Item::Oneof::DOUBLE);
    DOCTEST_MSGX_PARAMETERIZED_KWARGS("okk", "string", getString().asReader, Item::Oneof::STRING);

    mapping = msg.Mapping("ndarray"_kw = {1., 5., 8.});
    DOCTEST_MSGX_PARAMETERIZED_KWARGS(msgx::type::ndarray::DataType::FLOAT64, "ndarray",
                                      getNdArray().asReader().getDtype, Item::Oneof::ND_ARRAY);

    const std::vector<uint8_t> test = {5, 9, 89};
    mapping = msg.Mapping("test_uint8_vec"_kw = test);
    DOCTEST_MSGX_PARAMETERIZED_KWARGS(msgx::type::ndarray::DataType::U_INT8, "test_uint8_vec",
                                      getNdArray().asReader().getDtype, Item::Oneof::ND_ARRAY);

    //    msg.

    //    CHECK_EQ()
}
