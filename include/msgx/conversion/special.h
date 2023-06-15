#pragma once

#include "msgx/opaque_item/item.h"

namespace msgx
{

struct OpaqueMapping;

namespace conversion
{

// identity function that passthrough its input
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline void opaque_item(msgx::BindableOpaqueItem &item, std::shared_ptr<Derived> rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque shared ptr");
    item.set_assignment_callback([rhs_item](OpaqueItemBuilder builder) { rhs_item->build(builder); });
}

// identity function that passthrough its input, for unique_ptr
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline void opaque_item(msgx::BindableOpaqueItem &item, std::unique_ptr<Derived> rhs_item)
{
    SPDLOG_DEBUG("[Conversion] rebinding opaque unique ptr to shared ptr");
    // bind the unique ptr to shared ptr as std::function does not allow non-copyable objects
    std::shared_ptr<std::decay_t<Derived>> shared_ptr = std::move(rhs_item);
    opaque_item(item, std::move(shared_ptr));
}

// identity function that passthrough its input
inline void opaque_item(msgx::BindableOpaqueItem &item, msgx::OpaqueItem &rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque item");
    item.set_assignment_callback([&rhs_item](OpaqueItemBuilder builder) { rhs_item.build(builder); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, capnp::Orphan<type::Item::Oneof> orphan)
{
    SPDLOG_DEBUG("[Conversion] passthrough orphan of oneof with which: {}", orphan.getReader().which());
    item.assign_orphan(std::move(orphan));
}

}  // namespace conversion
}  // namespace msgx