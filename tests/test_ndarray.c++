#include <doctest/doctest.h>

#include "msgx/message.h"

using namespace msgx;
using Which = msgx::type::Item::Oneof::Which;

TEST_CASE("Test with and without orphanage")
{
    bool has_orphanage;

    //    SUBCASE("without orphanage")
    //    {
    //        has_orphanage = false;
    //    }

    SUBCASE("with orphanage")
    {
        has_orphanage = true;
    }
    capnp::MallocMessageBuilder msg_builder;
    auto mapping = std::make_unique<OpaqueMapping>();
    if (has_orphanage)
        mapping = std::make_unique<OpaqueMapping>([&msg_builder]() { return msg_builder.getOrphanage(); });

    (*mapping)["my_key"] = {1, -42};
    auto &my_key_ptr = mapping->get("my_key");
    CHECK(my_key_ptr);
    auto &downcast = dynamic_cast<BindableOpaqueItem &>(*my_key_ptr);
    if (has_orphanage)
        CHECK(!downcast.has_orphan());
    else
        CHECK(downcast.has_orphan());

    // both of them should still allow us to build
    OpaqueItemBuilder builder = msg_builder.initRoot<msgx::type::Item::Oneof>();
    downcast.build(builder);
    CHECK_EQ(builder.which(), Which::ND_ARRAY);
    auto reader = builder.getNdArray();
    const int *data_ptr = reinterpret_cast<const int *>(reader.getBuffer().asReader().begin());
    CHECK_EQ(reader.which(), msgx::type::ndarray::NdArray::Which::ONE_DIMENSIONAL);
    CHECK_EQ(reader.getOneDimensional(), 2);
    CHECK_EQ(data_ptr[0], 1);
    CHECK_EQ(data_ptr[1], -42);
}

#define DOCTEST_MSGX_PARAMETERIZED_1D_LIST(VALUE, ONEOF_GETTER, VALUE_TYPE, CHECK_TRUE_STR)                            \
    DOCTEST_SUBCASE((std::string("Test storing 1d array type [" #VALUE_TYPE "]").c_str()))                             \
    {                                                                                                                  \
        mapping["my_key"] = VALUE;                                                                                     \
        auto &my_key_ptr = mapping.get("my_key");                                                                      \
        CHECK(my_key_ptr);                                                                                             \
        auto &downcast = dynamic_cast<BindableOpaqueItem &>(*my_key_ptr);                                              \
        CHECK(!downcast.has_orphan());                                                                                 \
        OpaqueItemBuilder builder = msg_builder.initRoot<msgx::type::Item::Oneof>();                                   \
        downcast.build(builder);                                                                                       \
        CHECK_EQ(builder.which(), VALUE_TYPE);                                                                         \
        auto buffer = builder.ONEOF_GETTER();                                                                          \
        auto buf_reader = buffer.asReader();                                                                           \
        CHECK(CHECK_TRUE_STR);                                                                                         \
    }

// #define COMMA ,
//
// TEST_CASE("Test storing composed item")
//{
//     capnp::MallocMessageBuilder msg_builder;
//     OpaqueMapping mapping{[&msg_builder]() { return msg_builder.getOrphanage(); }};
//
//     using msgx::conversion::
//
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST({1 COMMA 2 COMMA 58 COMMA - 100), getIntList, Which::INT_LIST,
//                                        (buf_reader[3] == -100));
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST({5. COMMA - 1e-16 COMMA 5e37), getDoubleList, Which::DOUBLE_LIST,
//                                        (buf_reader[2] == doctest::Approx(5e37)));
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST({5.f COMMA - .01f), getFloatList, Which::FLOAT_LIST,
//                                        (buf_reader[1] == doctest::Approx(-.01f)));
//     std::vector<short> values = {1, 5, 10};
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(values), getIntList, Which::INT_LIST, true);
//
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(std::vector<unsigned short>{1 COMMA 2), getIntList, Which::INT_LIST,
//                                        true);
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(std::vector<unsigned char>{1 COMMA 2), getIntList, Which::INT_LIST,
//                                        true);
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(std::vector<long>{1 COMMA 2), getLongList, Which::LONG_LIST, true);
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(std::vector<unsigned int>{1 COMMA 2), getLongList, Which::LONG_LIST,
//                                        true);
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST(std::vector<unsigned long>{1 COMMA 2), getLongList, Which::LONG_LIST,
//                                        true);
//
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST({true COMMA false), getBoolList, Which::BOOL_LIST,
//                                        (buf_reader[1] == false));
//
//     DOCTEST_MSGX_PARAMETERIZED_1D_LIST({"true" COMMA "meme"), getStringList, Which::STRING_LIST,
//                                        (buf_reader[1] == "meme"));
// }
