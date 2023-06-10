#pragma once

#include <soraxas_toolbox/print_utils.h>

#include <functional>
#include <utility>

#include "msgx.capnp.h"
#include "msgx/opaque_item/conversion.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/mappings.h"

namespace msgx
{

class Messagerienst
{
public:
    static std::string &remote_address()
    {
        static std::string addr = "tcp://127.0.0.1:5557";
        return addr;
    }
};

template <typename T>
class Primitive
{
public:
    explicit Primitive(const T value) : value_(value)
    {
    }

private:
    T value_;
};

class Any
{
public:
};

}  // namespace msgx