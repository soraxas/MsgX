#include "test_common.h"

#include <array>

TEST_CASE("Test building any list")
{
    MessageX msg;

    msg["anylist"] = msg.AnyList("ha", 123, 5.5, msg.AnyList(65, "eirn", AsList({5.5, 65.})));

    //
    //    // both of them should still allow us to build
    //    OpaqueItemBuilder builder = msg_builder.initRoot<msgx::type::Item::Oneof>();
    //    downcast.build(builder);
    //    CHECK_EQ(builder.which(), Which::INT_LIST);
    //    auto reader = builder.getIntList().asReader();
    //    CHECK_EQ(reader.size(), 2);
    //    CHECK_EQ(reader[0], 1);
    //    CHECK_EQ(reader[1], -42);
}


TEST_CASE("Test building primitive std::array")
{
    MessageX msg;

    std::array<int, 3> haha {1, 2, 3};

    auto mapping = msg.getLinkedItemPtr<OpaqueMapping>();
    (*mapping)["array int"] = haha;
    OpaqueItemBuilder builder = get_oneof_builder(msg);
    auto downcast = downcast_bindable_item(mapping->get("array int").get());
    downcast->build(builder);
    CHECK_EQ(builder.which(), Which::ND_ARRAY);
}

TEST_CASE("Test building composite std::array")
{
    MessageX msg;

    std::array<std::vector<int>, 3> haha {};

    auto mapping = msg.getLinkedItemPtr<OpaqueMapping>();
    (*mapping)["array int"] = haha;
    OpaqueItemBuilder builder = get_oneof_builder(msg);
    auto downcast = downcast_bindable_item(mapping->get("array int").get());
    downcast->build(builder);
    CHECK_EQ(builder.which(), Which::ANY_LIST);
}
