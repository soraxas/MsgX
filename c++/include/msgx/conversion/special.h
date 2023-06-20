#pragma once

#include "msgx/dispatcher/builder_adopt_dispatcher.h"
#include "msgx/opaque_item/item.h"

namespace msgx
{

struct OpaqueMapping;

//// identity function that passthrough its input

// identity function that passthrough its input, for shared_ptr
template <typename Derived>
struct conversion<std::shared_ptr<Derived>,
                  typename std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, std::shared_ptr<Derived> rhs_item)
    {
        SPDLOG_DEBUG("[Conversion] passthrough opaque shared ptr");
        rhs_item->build(builder);
    }
};

// identity function that passthrough its input, for unique ptr (NOTE, it is not directly assignable, i.e., no copy)
template <typename Derived>
struct conversion<std::unique_ptr<Derived>,
                  typename std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
{
    static void convert(OpaqueItemBuilder builder, std::unique_ptr<Derived> rhs_item)
    {
        SPDLOG_DEBUG("[Conversion] rebinding opaque unique ptr to shared ptr");
        rhs_item->build(builder);
    }
};

template <>
struct conversion<capnp::Orphan<type::Item::Oneof>>
{
    static void convert(OpaqueItemBuilder builder, capnp::Orphan<type::Item::Oneof> orphan)
    {
        SPDLOG_DEBUG("[Conversion] passthrough orphan of oneof with which: {}", orphan.getReader().which());

        detail::builder_adopt_orphan(builder, std::move(orphan));
    }
};

}  // namespace msgx