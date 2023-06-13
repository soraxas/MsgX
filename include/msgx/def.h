#pragma once

#include <capnp/message.h>

#include <functional>

#include "msgx.capnp.h"

namespace msgx
{

using OrphanageGetter = std::function<capnp::Orphanage(void)>;
using OpaqueItemBuilder = ::msgx::type::Item::Oneof::Builder;

}  // namespace msgx