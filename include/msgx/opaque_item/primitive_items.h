#pragma once

#include "items.h"

namespace msgx
{

namespace conversion
{
class Null : public OpaqueItem
{
public:
    explicit Null() = default;

    void build(msgx::Item::Oneof::Builder builder) override
    {
        builder.setNull();
    }
};

class Bool : public OpaqueItem
{
public:
    explicit Bool(const bool value) : value_(value)
    {
    }

    void build(msgx::Item::Oneof::Builder builder) override
    {
        builder.setBool(value_);
    }

private:
    bool value_;
};

class String : public OpaqueItem
{
public:
    explicit String(std::string value) : value_(std::move(value))
    {
    }

    void build(msgx::Item::Oneof::Builder builder) override
    {
        builder.setString(value_);
    }

private:
    std::string value_;
};

}  // namespace details

}  // namespace msgx