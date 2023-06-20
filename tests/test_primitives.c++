#include <doctest/doctest.h>

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
