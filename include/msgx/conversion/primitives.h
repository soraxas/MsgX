#pragma once

#include "msgx/opaque_item/item.h"

namespace msgx
{
namespace conversion
{

inline void opaque_item(msgx::BindableOpaqueItem &item, std::nullptr_t)
{
    SPDLOG_DEBUG("[Conversion] nullptr");
    item.set_assignment_callback([](OpaqueItemBuilder builder) { builder.setNull(); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const bool value)
{
    SPDLOG_DEBUG("[Conversion] bool '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setBool(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const std::string &value)
{
    SPDLOG_DEBUG("[Conversion] std::string '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const char *value)
{
    SPDLOG_DEBUG("[Conversion] const char '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
}

// identity function that passthrough its input
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline void opaque_item(msgx::BindableOpaqueItem &item, std::shared_ptr<Derived> rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque ptr");
    item.set_assignment_callback([rhs_item](OpaqueItemBuilder builder) { rhs_item->build(builder); });
}

// identity function that passthrough its input
inline void opaque_item(msgx::BindableOpaqueItem &item, msgx::OpaqueItem &rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque ptr");
    item.set_assignment_callback([&rhs_item](OpaqueItemBuilder builder) { rhs_item.build(builder); });
}

/*
 * ===== signed =====
 */
inline void opaque_item(msgx::BindableOpaqueItem &item, const char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setChar(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const int16_t value)
{
    SPDLOG_DEBUG("[Conversion] int16_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setShort(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const int32_t value)
{
    SPDLOG_DEBUG("[Conversion] int32_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setInt(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const int64_t value)
{
    SPDLOG_DEBUG("[Conversion] int64_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setLong(value); });
}

/*
 * ===== unsigned =====
 */
inline void opaque_item(msgx::BindableOpaqueItem &item, const unsigned char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUChar(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const uint16_t value)
{
    SPDLOG_DEBUG("[Conversion] uint16_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUShort(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const uint32_t value)
{
    SPDLOG_DEBUG("[Conversion] uint32_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUInt(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const uint64_t value)
{
    SPDLOG_DEBUG("[Conversion] uint64_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setULong(value); });
}

/*
 * ===== floating point =====
 */
inline void opaque_item(msgx::BindableOpaqueItem &item, const float_t value)
{
    SPDLOG_DEBUG("[Conversion] float_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setFloat(value); });
}

inline void opaque_item(msgx::BindableOpaqueItem &item, const double_t value)
{
    SPDLOG_DEBUG("[Conversion] double_t '{}'", value);
    item.set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setDouble(value); });
}

}  // namespace conversion
}  // namespace msgx