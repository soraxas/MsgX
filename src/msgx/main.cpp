#include <capnp/compat/std-iterator.h>
#include <capnp/message.h>
#include <capnp/serialize-packed.h>

#include "msgx/message.h"

// #include "_impl/core.hpp"

#include <soraxas_toolbox/print_utils.h>

#include "message.h"
#include "msgx.capnp.h"
#include "msgx/comms/zmq.h"

namespace msgx
{
using MessageBuilderPtr = std::unique_ptr<capnp::MallocMessageBuilder>;

class Message
{
public:
    explicit Message()
    {
        msgx::type::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::type::NdArray<float>>();

        auto shapeBuilder =
            msgx::helpers::build_capnp_list<uint>(array_builder.initMultiDimensional().initShape(2), {1, 2});
    }

    //        msgx::type::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::type::NdArray<float>>();

    void send(zmq::send_flags send_flags = zmq::send_flags::dontwait);

    capnp::MallocMessageBuilder msgBuilder_;
    bool already_sent_{false};
};

std::string operator""_a(const char *str, std::size_t)
{
    return std::string{str} + str;
}

void Message::send(zmq::send_flags send_flags)
{
    sxs::println("okok ", "ha"_a);
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
    auto item_builder = msgBuilder_.initRoot<msgx::type::Item>();
    //        auto array_builder = item_builder.initValue().initFloatArray();
    //
    ////        msgx::type::NdArray<float>::Builder array_builder = msgBuilder_.initRoot<msgx::type::NdArray<float>>();
    //
    //        auto shapeBuilder =
    //        msgx::helpers::build_capnp_list<uint>(array_builder.initMultiDimensional().initShape(2), {1, 2});
    //
    //        array_builder.initBuffer(5);

    MessageX msg;

    //    auto builder = msg.root_;

    //    msgx::conversion::opaque_item(builder, "ha");
    //    msgx::conversion::opaque_item(builder, 'f');
    //
    //
    //    std::vector<int> ars = {1,2};
    //    msgx::conversion::opaque_item(builder, ars);
    //    msgx::conversion::opaque_item<float>(builder, {33,4.,2234.3});

    msg = "ha";

    msg = {1, 2, 12};

    ////    auto _mappings = OpaqueMapping{};
    //    auto _mappings = std::make_shared<OpaqueMapping>();
    //
    //    (*_mappings)["q"] = true;
    //
    msg["ha"] = "ok";
    msg["ha"] = msg.Mapping();
    msg["ha"] = msg.container("haha");
    //
    //    auto ha = msg.lolHAHAcontainer();

    auto haha = std::make_shared<msgx::BindableOpaqueItem>(msg.get_orphanage_getter());

    msgx::conversion::opaque_item(*haha, 1.2);

    msgx::conversion::opaque_item(*haha, {2., 2.32, 3., 1., 2.});
    msgx::conversion::opaque_item(*haha, "{2.,2.32,3.,1.,2.}");
    msgx::conversion::opaque_item(*haha, "{2.,2.32,3.,1.,2.}");
    msgx::conversion::opaque_item(*haha, "{2.,2.32,3.,1.,2.}");
    msgx::conversion::opaque_item(*haha, "{2.,2.32,3.,1.,2.}");
    msgx::conversion::opaque_item(*haha, "{2.,2.32,3.,1.,2.}");

    //    msg = 3;

    //    msg["ok"] = ;

    msg = haha;

    //    msg = ha;

    msg.send();

    msg.container("ha");

    std::function<capnp::Orphanage(void)> getter = [this]() { return msgBuilder_.getOrphanage(); };

    auto mappings = OpaqueMapping{getter};

    //    msgBuilder_.getOrphanage();

    using dtype = double;
    //    using dtype = int;

    std::vector<dtype> isren = {1, 2, 5, 332, 2323};
    //

    mappings["true"] = msg.container("isren");

    mappings["okok bye"] = isren;
    mappings["_"] = isren;
    mappings["_"] = true;

    std::initializer_list<float> rsie = {1, 2, 5, 332, 23233};
    ;

    //    msgx::conversion::OpaqueArray<float> a{};

    //    auto vv = std::make_shared<msgx::conversion::OpaqueArray<float>>();
    //    auto vv = msgx::conversion::opaque_item(getter, {10010, 2, 5, 332, 23233, 3223, 92012});

    //    vv->assign({1, 23, 2});

    //    mappings["rsieokok"] = vv;
    //    ;

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
    msgx::type::NdArray<float>::Builder array_builder = message.initRoot<msgx::type::NdArray<float>>();

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
