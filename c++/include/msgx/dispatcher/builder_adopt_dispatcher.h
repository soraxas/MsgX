#pragma once

#include "msgx/conversion/common.h"
#include "msgx/opaque_item/bindable_item.h"

namespace msgx
{
namespace detail
{

void builder_adopt_orphan(OpaqueItemBuilder builder, capnp::Orphan<type::Item::Oneof> orphan);

}  // namespace detail
}  // namespace msgx