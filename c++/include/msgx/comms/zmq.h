#pragma once

#include <memory>
#include <thread>
#include <zmq.hpp>

#include "comms.h"

namespace msgx
{
namespace comms
{
namespace detail
{
struct BindedZmq;
}

class Zmq
{
public:
    /**
     * Singletons should not be cloneable.
     */
    Zmq(Zmq &other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const Zmq &) = delete;

public:
    static detail::BindedZmq &get_instance();

    static void Send(capnp::MallocMessageBuilder &msg_builder);

    void send(capnp::MallocMessageBuilder &msg_builder);

protected:
    explicit Zmq(size_t sleep_after_bind = 1000);

    zmq::context_t context_;
    zmq::socket_t publisher_;

    friend detail::BindedZmq;
};

namespace detail
{
struct BindedZmq
{
    template <class... Args>
    explicit BindedZmq(Args... args) : zmq_instance(std::forward<Args>(args)...)
    {
    }

    std::string binded_addr;
    Zmq zmq_instance;
};
}  // namespace detail

}  // namespace comms

}  // namespace msgx