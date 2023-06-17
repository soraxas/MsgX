#pragma once

#include <capnp/message.h>
#include <capnp/serialize.h>

#include <string>

namespace msgx
{

namespace comms
{

inline std::string &remote_address()
{
    static std::string addr = "tcp://127.0.0.1:5558";
    return addr;
}

}  // namespace comms

}  // namespace msgx