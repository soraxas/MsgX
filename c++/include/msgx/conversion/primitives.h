#pragma once

#include "msgx/conversion/convertible.h"
#include "msgx/opaque_item/item.h"

namespace msgx
{

template <>
struct conversion<std::nullptr_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, std::nullptr_t)
    {
        SPDLOG_DEBUG("[Conversion] nullptr");
        builder.setNull();
    }
};

template <>
struct conversion<bool>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const bool value)
    {
        SPDLOG_DEBUG("[Conversion] bool '{}'", value);
        builder.setBool(value);
    }
};

template <>
struct conversion<std::string &>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const std::string &value)
    {
        SPDLOG_DEBUG("[Conversion] std::string '{}'", value);
        builder.setString(value);
    }
};

template <>
struct conversion<const char *>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const char *value)
    {
        SPDLOG_DEBUG("[Conversion] const char '{}'", value);
        builder.setString(value);
    }
};

/*
 * ===== signed =====
 */
template <>
struct conversion<char>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const char value)
    {
        SPDLOG_DEBUG("[Conversion] char '{}'", value);
        builder.setChar(value);
    }
};

template <>
struct conversion<int16_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const int16_t value)
    {
        SPDLOG_DEBUG("[Conversion] int16_t '{}'", value);
        builder.setShort(value);
    }
};

template <>
struct conversion<int32_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const int32_t value)
    {
        SPDLOG_DEBUG("[Conversion] int32_t '{}'", value);
        builder.setInt(value);
    }
};

template <>
struct conversion<int64_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const int64_t value)
    {
        SPDLOG_DEBUG("[Conversion] int64_t '{}'", value);
        builder.setLong(value);
    }
};

/*
 * ===== unsigned =====
 */
template <>
struct conversion<unsigned char>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const unsigned char value)
    {
        SPDLOG_DEBUG("[Conversion] char '{}'", value);
        builder.setUChar(value);
    }
};

template <>
struct conversion<uint16_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const uint16_t value)
    {
        SPDLOG_DEBUG("[Conversion] uint16_t '{}'", value);
        builder.setUShort(value);
    }
};

template <>
struct conversion<uint32_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const uint32_t value)
    {
        SPDLOG_DEBUG("[Conversion] uint32_t '{}'", value);
        builder.setUInt(value);
    }
};

template <>
struct conversion<uint64_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const uint64_t value)
    {
        SPDLOG_DEBUG("[Conversion] uint64_t '{}'", value);
        builder.setULong(value);
    }
};

/*
 * ===== floating point =====
 */
template <>
struct conversion<float_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const float_t value)
    {
        SPDLOG_DEBUG("[Conversion] float_t '{}'", value);
        builder.setFloat(value);
    }
};

template <>
struct conversion<double_t>
{
    static constexpr bool directly_assignable = true;

    static void convert(OpaqueItemBuilder builder, const double_t value)
    {
        SPDLOG_DEBUG("[Conversion] double_t '{}'", value);
        builder.setDouble(value);
    }
};

}  // namespace msgx