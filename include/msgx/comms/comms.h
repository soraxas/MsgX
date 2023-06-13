#pragma once

#include <capnp/message.h>
#include <capnp/serialize.h>

#include <string>

namespace msgx
{

namespace comms
{

class Comms
{
public:
    static std::string &remote_address()
    {
        static std::string addr = "tcp://127.0.0.1:5557";
        return addr;
    }

    virtual void send(capnp::MallocMessageBuilder &) = 0;
};

}  // namespace comms

}  // namespace msgx