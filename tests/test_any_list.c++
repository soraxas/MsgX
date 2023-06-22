#include "test_common.h"

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
