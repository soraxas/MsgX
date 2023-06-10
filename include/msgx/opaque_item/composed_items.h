#pragma once

#include <soraxas_toolbox/print_utils.h>

#include <vector>

#include "msgx/capnp_conversion.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/opaque_dispatcher.h"

namespace msgx
{

namespace conversion
{

using OrphanageGetter = std::function<capnp::Orphanage(void)>;

template <typename DataType>
class OpaqueArray : public OpaqueComposedItem<msgx::NdArray<capnp::List<DataType>>>
{
public:
    using NdArrayType = msgx::NdArray<capnp::List<DataType>>;
    using CapnpMsgType = NdArrayType;

    explicit OpaqueArray(std::function<capnp::Orphanage()> get_orphanage_functor, const std::vector<DataType> &values)
      : OpaqueComposedItem<NdArrayType>(get_orphanage_functor)
    {
        auto builder = this->createOrphanGetBuilder();
        msgx::helpers::build_capnp_list<DataType>(builder.initBuffer(values.size()), values);
    }

    void build(msgx::Item::Oneof::Builder builder) override
    {
        // the template argument will dispatch and retrieve different method pointer
        // depending on the data type
        (builder.*NdArrayAdoptOrphanDispatcher<DataType>())(std::move(this->orphan_));
    }
};

}  // namespace conversion

}  // namespace msgx