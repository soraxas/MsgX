#pragma once

#include <doctest/doctest.h>

#include "msgx/message.h"

using msgx::type::Item;
using Which = msgx::type::Item::Oneof::Which;
using namespace msgx;

inline OpaqueItemBuilder get_oneof_builder(const MessageX &msg)
{
    return std::move(msg.get_orphanage_getter()().newOrphan<msgx::type::Item::Oneof>().get());
}

inline OpaqueItemBuilder get_oneof_builder(capnp::MallocMessageBuilder &msg_builder)
{
    return msg_builder.initRoot<msgx::type::Item::Oneof>();
}

inline BindableOpaqueItem *downcast_bindable_item(OpaqueItem *raw_ptr)
{
    auto downcast = dynamic_cast<BindableOpaqueItem *>(raw_ptr);
    CHECK((downcast != nullptr));
    return downcast;
}