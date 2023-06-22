#include "test_common.h"

TEST_CASE("Test with and without orphanage")
{
    MessageX msg;
    auto mapping = msg.getLinkedItemPtr<OpaqueMapping>();

    (*mapping)["my_key"] = AsList({1, -42});
    auto &my_key_ptr = mapping->get("my_key");
    CHECK(my_key_ptr);
    auto &downcast = *downcast_bindable_item(my_key_ptr.get());
    CHECK(!downcast.has_orphan());

    // both of them should still allow us to build
    OpaqueItemBuilder builder = get_oneof_builder(msg);
    downcast.build(builder);
    CHECK_EQ(builder.which(), Which::INT_LIST);
    auto reader = builder.getIntList().asReader();
    CHECK_EQ(reader.size(), 2);
    CHECK_EQ(reader[0], 1);
    CHECK_EQ(reader[1], -42);
}

#define DOCTEST_MSGX_PARAMETERIZED_1D_LIST(VALUE, ONEOF_GETTER, VALUE_TYPE, CHECK_TRUE_STR)                            \
    /* DOCTEST_SUBCASE((std::string("Test storing 1d array type [" #VALUE_TYPE "]").c_str())) */                       \
    {                                                                                                                  \
        (*mapping)["my_key"] = VALUE;                                                                                  \
        auto &my_key_ptr = mapping->get("my_key");                                                                     \
        CHECK(my_key_ptr);                                                                                             \
        auto &downcast = *downcast_bindable_item(my_key_ptr.get());                                                    \
        CHECK(!downcast.has_orphan());                                                                                 \
        OpaqueItemBuilder builder = get_oneof_builder(msg);                                                            \
        downcast.build(builder);                                                                                       \
        CHECK_EQ(builder.which(), VALUE_TYPE);                                                                         \
        auto buffer = builder.ONEOF_GETTER();                                                                          \
        auto buf_reader = buffer.asReader();                                                                           \
        CHECK(CHECK_TRUE_STR);                                                                                         \
    }

#define COMMA ,

TEST_CASE("Test storing composed item")
{
    MessageX msg;
    auto mapping = msg.getLinkedItemPtr<OpaqueMapping>();

    using msgx::AsList;

    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList({1 COMMA 2 COMMA 58 COMMA - 100}), getIntList, Which::INT_LIST,
                                       (buf_reader[3] == -100));
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList({5. COMMA - 1e-16 COMMA 5e37}), getDoubleList, Which::DOUBLE_LIST,
                                       (buf_reader[2] == doctest::Approx(5e37)));
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList({5.f COMMA - .01f}), getFloatList, Which::FLOAT_LIST,
                                       (buf_reader[1] == doctest::Approx(-.01f)));
    std::vector<short> values = {1, 5, 10};
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(values), getIntList, Which::INT_LIST, true);

    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(std::vector<unsigned short>{1 COMMA 2}), getIntList, Which::INT_LIST,
                                       true);
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(std::vector<unsigned char>{1 COMMA 2}), getIntList, Which::INT_LIST,
                                       true);
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(std::vector<long>{1 COMMA 2}), getLongList, Which::LONG_LIST, true);
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(std::vector<unsigned int>{1 COMMA 2}), getLongList, Which::LONG_LIST,
                                       true);
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList(std::vector<unsigned long>{1 COMMA 2}), getLongList, Which::LONG_LIST,
                                       true);

    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList({true COMMA false}), getBoolList, Which::BOOL_LIST,
                                       (buf_reader[1] == false));

    DOCTEST_MSGX_PARAMETERIZED_1D_LIST(AsList({"true" COMMA "meme"}), getStringList, Which::STRING_LIST,
                                       (buf_reader[1] == "meme"));
    DOCTEST_MSGX_PARAMETERIZED_1D_LIST({"true" COMMA "meme"}, getAnyList, Which::ANY_LIST,
                                       (buf_reader[1].getOneof().getString() == "meme"));
}

TEST_CASE("Test storing composed item")
{
    capnp::MallocMessageBuilder msg_builder;

    auto builder = msg_builder.initRoot<msgx::type::Item>();
    auto builder2 = msg_builder.initRoot<msgx::type::Item>();
    auto builder3 = msg_builder.initRoot<msgx::type::Item>();

    auto anyarraybuf = builder3.initOneof().initAnyList(5);

    anyarraybuf.setWithCaveats(0, builder);

    auto a = {builder, builder2};
}
