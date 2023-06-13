#pragma once

#include "msgx/opaque_item/items.h"

namespace msgx
{
namespace conversion
{

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, std::nullptr_t)
{
    SPDLOG_DEBUG("[Conversion] nullptr");
    item->set_assignment_callback([](OpaqueItemBuilder builder) { builder.setNull(); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const bool value)
{
    SPDLOG_DEBUG("[Conversion] bool '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setBool(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const std::string &value)
{
    SPDLOG_DEBUG("[Conversion] std::string '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const char *value)
{
    SPDLOG_DEBUG("[Conversion] const char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
    return item;
}

// identity function that passthrough its input
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, std::shared_ptr<Derived> rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque ptr");
    item->set_assignment_callback([rhs_item](OpaqueItemBuilder builder) { rhs_item->build(builder); });
    return item;
}

/*
 * ===== signed =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setChar(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int16_t value)
{
    SPDLOG_DEBUG("[Conversion] int16_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setShort(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int32_t value)
{
    SPDLOG_DEBUG("[Conversion] int32_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setInt(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int64_t value)
{
    SPDLOG_DEBUG("[Conversion] int64_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setLong(value); });
    return item;
}

/*
 * ===== unsigned =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const unsigned char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUChar(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint16_t value)
{
    SPDLOG_DEBUG("[Conversion] uint16_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUShort(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint32_t value)
{
    SPDLOG_DEBUG("[Conversion] uint32_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUInt(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint64_t value)
{
    SPDLOG_DEBUG("[Conversion] uint64_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setULong(value); });
    return item;
}

/*
 * ===== floating point =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const float_t value)
{
    SPDLOG_DEBUG("[Conversion] float_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setFloat(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const double_t value)
{
    SPDLOG_DEBUG("[Conversion] double_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setDouble(value); });
    return item;
}

}  // namespace conversion
}  // namespace msgx