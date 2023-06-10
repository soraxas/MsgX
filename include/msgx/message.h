#pragma once

#include <capnp/message.h>

namespace msgx
{
class Message
{
protected:
    capnp::MallocMessageBuilder msg_builder_;
};

}  // namespace msgx