#pragma once

#include <capnp/message.h>

#include <functional>

#include "msgx.capnp.h"

#ifdef BUILD_WITH_EASYSPDLOG
// #include <spdlog/spdlog.h>
#define EASYSPDLOG_AUTO_SETUP
#include <easyspdlog.h>
#else
#define SPDLOG_ERROR(...)
#define SPDLOG_DEBUG(...)
#define SPDLOG_TRACE(...)
#endif

namespace msgx
{

typedef unsigned char byte;

using OrphanageGetter = std::function<capnp::Orphanage(void)>;
using OpaqueItemBuilder = ::msgx::type::Item::Oneof::Builder;

}  // namespace msgx