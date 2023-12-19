#pragma once

#include <memory>
#include <thread>
#include <zmq.hpp>

#include "comms.h"

namespace msgx
{
namespace comms
{

// clang-format off
namespace detail { struct BindedZmq; }  // forward declare
// clang-format on

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
    static detail::BindedZmq *get_instance();

    static void Send(capnp::MallocMessageBuilder &msg_builder);

    void send(capnp::MallocMessageBuilder &msg_builder);

    bool degraded()
    {
        return degraded_;
    }

protected:
    explicit Zmq(size_t sleep_after_bind = 1000) noexcept;

    zmq::context_t context_;
    zmq::socket_t publisher_;

    bool degraded_;

    friend detail::BindedZmq;
};

namespace detail
{
struct BindedZmq
{
    template <class... Args>
    explicit BindedZmq(Args... args)
      : zmq_instance(std::forward<Args>(args)...), binded_addr(::msgx::comms::remote_address())
    {
    }

    bool degraded()
    {
        return zmq_instance.degraded();
    }

    std::string binded_addr;
    Zmq zmq_instance;
};
}  // namespace detail

}  // namespace comms

}  // namespace msgx