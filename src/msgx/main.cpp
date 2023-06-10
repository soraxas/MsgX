#include <capnp/compat/std-iterator.h>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>

// #include "_impl/core.hpp"

#include <soraxas_toolbox/print_utils.h>

#include "comms/zmq.h"
#include "message.h"
#include "msgx.capnp.h"
#include "msgx/capnp_conversion.h"

namespace msgx
{
using MessageBuilderPtr = std::unique_ptr<capnp::MallocMessageBuilder>;

class Message
{
public:
    explicit Message()
    {
        msgx::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::NdArray<float>>();

        auto shapeBuilder =
            msgx::helpers::build_capnp_list<uint>(array_builder.initMultiDimensional().initShape(2), {1, 2});
    }

    //        msgx::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::NdArray<float>>();

    void send(zmq::send_flags send_flags = zmq::send_flags::dontwait);

    capnp::MallocMessageBuilder msgBuilder_;
    bool already_sent_{false};
};

void Message::send(zmq::send_flags send_flags)
{
    if (!already_sent_)
    {
        already_sent_ = true;
    }
    else
    {
        std::cerr << "[WARN] this message has already been allocated and sent. "
                     "This will leads to growing size of capnp message."
                  << std::endl;
    }
    auto item_builder = msgBuilder_.initRoot<msgx::Item>();
    //        auto array_builder = item_builder.initValue().initFloatArray();
    //
    ////        msgx::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::NdArray<float>>();
    //
    //        auto shapeBuilder =
    //        msgx::helpers::build_capnp_list<uint>(array_builder.initMultiDimensional().initShape(2), {1, 2});
    //
    //        array_builder.initBuffer(5);

    std::function<capnp::Orphanage(void)> getter = [this]() { return msgBuilder_.getOrphanage(); };

    auto mappings = OpaqueMapping{getter};

    mappings["ha"] = false;
    mappings["ha5"] = false;
    mappings[true] = false;
    mappings["ha|^{%N"]["r"][false][true] = true;
    mappings[true] = nullptr;

    //    msgBuilder_.getOrphanage();

    using dtype = double;
    //    using dtype = int;

    std::vector<dtype> isren = {1, 2, 5, 332, 2323};
    //
    mappings["okok"] = isren;

    std::initializer_list<float> rsie = {1, 2, 5, 332, 23233};
    ;

    auto vv = msgx::conversion::opaque_item(getter, {10010, 2, 5, 332, 23233, 3223, 92012});

    mappings["rsieokok"] = vv;
    ;

    mappings.build(item_builder.initOneof());

    sxs::println("okk");

    //        item_builder.initValue().setString("ye");

    comms::Zmq::Send(msgBuilder_);

    sxs::println("bye");
    //        initialise_publisher();
    //
    //        auto wordArray = capnp::messageToFlatArray(*msgBuilder_);
    //        zmq::message_t zmq_msg{wordArray.begin(), wordArray.size() * sizeof(capnp::word), NULL};
    //        static_publisher->send(zmq_msg, send_flags);
    //
    //        reset();
    //        msgBuilder_ = std::make_unique<capnp::MallocMessageBuilder>(wordArray);
}

using namespace sxs;

void add()
{
    capnp::MallocMessageBuilder message;
    msgx::NdArray<float>::Builder array_builder = message.initRoot<msgx::NdArray<float>>();

    auto shapeBuilder =
        msgx::helpers::build_capnp_list<uint>(array_builder.initMultiDimensional().initShape(2), {1, 2});

    println(shapeBuilder.asReader());
    println(msgx::helpers::get_capnp_list<float>(shapeBuilder.asReader()));
    //    println(*a.asReader().begin());

    //    shape.

    for (int i = 0; i < 20; ++i)
    {
        Message m;

        m.send();
        break;
    }
}

}  // namespace msgx
