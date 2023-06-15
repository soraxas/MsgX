#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#include "msgx/message.h"

using namespace msgx;
using Which = msgx::type::Item::Oneof::Which;

// check key is correct, and orphan is null, while we can retrieve value with builder
#define DOCTEST_MSGX_PARAMETERIZED_CUSTOM_CHECKING(VALUE, ONEOF_GETTER, VALUE_TYPE, CHECK_TRUE_STR)                    \
    DOCTEST_SUBCASE((std::string("Test storing primitive type [" #VALUE_TYPE "]").c_str()))                            \
    {                                                                                                                  \
        mapping["my_key"] = VALUE;                                                                                     \
        auto &my_key_ptr = mapping.get("my_key");                                                                      \
        CHECK(my_key_ptr);                                                                                             \
        auto &downcast = dynamic_cast<BindableOpaqueItem &>(*my_key_ptr);                                              \
        CHECK(downcast.has_orphan());                                                                                  \
        OpaqueItemBuilder builder = msg_builder.initRoot<msgx::type::Item::Oneof>();                                   \
        downcast.build(builder);                                                                                       \
        CHECK_EQ(builder.which(), VALUE_TYPE);                                                                         \
        CHECK(CHECK_TRUE_STR);                                                                                         \
    }

// use the previous macro, but with just checking output is same as input
#define DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(VALUE, ONEOF_GETTER, VALUE_TYPE)                                          \
    DOCTEST_MSGX_PARAMETERIZED_CUSTOM_CHECKING(VALUE, ONEOF_GETTER, VALUE_TYPE, (builder.ONEOF_GETTER() == VALUE))

TEST_CASE("Test storing primitive")
{
    OpaqueMapping mapping;
    capnp::MallocMessageBuilder msg_builder;

    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(static_cast<unsigned char>('b'), getUChar, Which::U_CHAR);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(static_cast<unsigned short>(1), getUShort, Which::U_SHORT);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(1U, getUInt, Which::U_INT);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(50000UL, getULong, Which::U_LONG);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE('a', getChar, Which::CHAR);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(static_cast<short>(-4), getShort, Which::SHORT);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(1, getInt, Which::INT);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(1000L, getLong, Which::LONG);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(1.f, getFloat, Which::FLOAT);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(1., getDouble, Which::DOUBLE);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(true, getBool, Which::BOOL);
    DOCTEST_MSGX_PARAMETERIZED_PRIMITIVE(false, getBool, Which::BOOL);

    DOCTEST_MSGX_PARAMETERIZED_CUSTOM_CHECKING(nullptr, getNull, Which::NULL_, true);
    DOCTEST_MSGX_PARAMETERIZED_CUSTOM_CHECKING("HeLlo", getString, Which::STRING,
                                               std::string(builder.getString().cStr()) == "HeLlo");
}

TEST_CASE("Test with and without orphanage")
{
    bool has_orphanage;
    SUBCASE("without orphanage")
    {
        has_orphanage = false;
    }
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
    CHECK_EQ(builder.which(), Which::INT_ARRAY);
    auto reader = builder.getIntArray().getBuffer().asReader();
    CHECK_EQ(reader.size(), 2);
    CHECK_EQ(reader[0], 1);
    CHECK_EQ(reader[1], -42);
}

#define DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(VALUE, ONEOF_GETTER, VALUE_TYPE, CHECK_TRUE_STR)                           \
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
        CHECK_EQ(builder.which(), VALUE_TYPE);                                                                         \
        auto buffer = builder.ONEOF_GETTER().getBuffer();                                                              \
        CHECK_EQ(builder.ONEOF_GETTER().getOneDimensional(), ::capnp::VOID);                                           \
        auto buf_reader = buffer.asReader();                                                                           \
        CHECK(CHECK_TRUE_STR);                                                                                         \
    }

#define COMMA ,

TEST_CASE("Test storing composed item")
{
    capnp::MallocMessageBuilder msg_builder;
    OpaqueMapping mapping{[&msg_builder]() { return msg_builder.getOrphanage(); }};

    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY({1 COMMA 2 COMMA 58 COMMA - 100}, getIntArray, Which::INT_ARRAY,
                                        (buf_reader[3] == -100));
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY({5. COMMA - 1e-16 COMMA 5e37}, getDoubleArray, Which::DOUBLE_ARRAY,
                                        (buf_reader[2] == doctest::Approx(5e37)));
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY({5.f COMMA - .01f}, getFloatArray, Which::FLOAT_ARRAY,
                                        (buf_reader[1] == doctest::Approx(-.01f)));
    std::vector<short> values = {1, 5, 10};
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(values, getIntArray, Which::INT_ARRAY, true);

    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(std::vector<unsigned short>{1 COMMA 2}, getIntArray, Which::INT_ARRAY, true);
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(std::vector<unsigned char>{1 COMMA 2}, getIntArray, Which::INT_ARRAY, true);
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(std::vector<long>{1 COMMA 2}, getLongArray, Which::LONG_ARRAY, true);
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(std::vector<unsigned int>{1 COMMA 2}, getLongArray, Which::LONG_ARRAY, true);
    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY(std::vector<unsigned long>{1 COMMA 2}, getLongArray, Which::LONG_ARRAY, true);

    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY({true COMMA false}, getBoolArray, Which::BOOL_ARRAY, (buf_reader[1] == false));

    DOCTEST_MSGX_PARAMETERIZED_1D_ARRAY({"true" COMMA "meme"}, getStringArray, Which::STRING_ARRAY,
                                        (buf_reader[1] == "meme"));
}

TEST_CASE("Test storing composed item")
{
    capnp::MallocMessageBuilder msg_builder;
    OpaqueMapping mapping{[&msg_builder]() { return msg_builder.getOrphanage(); }};

    auto builder = msg_builder.initRoot<msgx::type::Item>();
    auto builder2 = msg_builder.initRoot<msgx::type::Item>();
    auto builder3 = msg_builder.initRoot<msgx::type::Item>();

    auto anyarray = builder3.initOneof().initAnyArray();
    auto anyarraybuf = anyarray.initBuffer(5);

    anyarraybuf.setWithCaveats(0, builder);

    auto a = {builder, builder2};
}
