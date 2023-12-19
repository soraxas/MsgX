#include "msgx/comms/zmq.h"

#include <iostream>

namespace msgx
{

namespace comms
{

Zmq::Zmq(size_t sleep_after_bind) noexcept : context_(), publisher_(context_, ZMQ_PUB), degraded_(false)
{
    try
    {
        publisher_.bind(::msgx::comms::remote_address());

        if (sleep_after_bind > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_after_bind));
    }
    catch (const zmq::error_t &err)
    {
        std::cerr << "[MsgX] error when binding zmq: " << err.what() << std::endl;
        degraded_ = true;
    }
}

detail::BindedZmq *Zmq::get_instance()
{
    static std::unique_ptr<detail::BindedZmq> instance_;
    if (!instance_ || instance_->binded_addr != ::msgx::comms::remote_address())
    {
        instance_ = std::make_unique<detail::BindedZmq>();
    }
    return instance_->degraded() ? nullptr : instance_.get();
}

void Zmq::send(capnp::MallocMessageBuilder &msg_builder)
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

void Zmq::Send(capnp::MallocMessageBuilder &msg_builder)
{
    auto instance = get_instance();
    if (instance)
    {
        instance->zmq_instance.send(msg_builder);
    }
}

}  // namespace comms
}  // namespace msgx