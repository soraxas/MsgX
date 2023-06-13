#pragma once

#include <memory>
#include <thread>
#include <zmq.hpp>

#include "comms.h"

namespace msgx
{

namespace comms
{

class Zmq : public Comms
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
    static Zmq &get_instance()
    {
        static std::unique_ptr<Zmq> instance_{new Zmq};
        return *instance_;
    }

    template <typename... Args>
    static void Send(Args &&...args)
    {
        get_instance().send(std::forward<Args>(args)...);
    }

    void send(capnp::MallocMessageBuilder &msg_builder) override
    {
        zmq::send_flags send_flags = zmq::send_flags::dontwait;
        auto wordArray = capnp::messageToFlatArray(msg_builder);

        // the following version will not copy memory, but use the supplied buffer directly
        // The downside is that zmq doesn't immediately send msg (it might wait in some queue),
        // which means this function will return before zmq actually managed to send the message.
        // HENCE, it will likely read memory that had been overwritten by something else.
        // IT HAD BEEN A FEW HOURS WASTED TRYIN DEBUG THE INVALID MEMORY ISSUE
        //                zmq::message_t zmq_msg{wordArray.begin(), wordArray.size() * sizeof(capnp::word), ZMQ_NULL,
        //                ZMQ_NULL};

        // the following will copy memory
        zmq::message_t zmq_msg{wordArray.begin(), wordArray.size() * sizeof(capnp::word)};

        publisher_.send(zmq_msg, send_flags);
    }

protected:
    explicit Zmq(size_t sleep_after_bind = 1000) : context_(), publisher_(context_, ZMQ_PUB)
    {
        publisher_.bind(remote_address());
        if (sleep_after_bind > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_after_bind));
    }

    zmq::context_t context_;
    zmq::socket_t publisher_;
};

}  // namespace comms

}  // namespace msgx