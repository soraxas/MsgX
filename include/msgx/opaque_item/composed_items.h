#pragma once

#include <soraxas_toolbox/print_utils.h>

#include <vector>

#include "msgx/capnp_conversion.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/opaque_dispatcher.h"

// namespace msgx
//{
//
// namespace conversion
//{
//
// using OrphanageGetter = std::function<capnp::Orphanage(void)>;
//
// template <typename DataType>
// class OpaqueArray : public OpaqueComposedItem<msgx::type::NdArray<capnp::List<DataType>>>
//{
// public:
//     using NdArrayType = msgx::type::NdArray<capnp::List<DataType>>;
//     using CapnpMsgType = NdArrayType;
//
//     using OpaqueComposedItem<NdArrayType>::OpaqueComposedItem;
//
//     void assign(const std::vector<DataType> &values)
//     {
//         if (this->has_orphan_getter())
//         {
//             auto builder = this->createOrphanGetBuilder();
//             assign_builder(builder);
//         }
//     }
//
//     void assign_builder(capnp::BuilderFor<CapnpMsgType> builder){
//         std::vector<DataType> values = {1,2.1,12312.12313};
//             msgx::helpers::build_capnp_list<DataType>(builder.initBuffer(values.size()), values);
//     }
//
//     void build(msgx::type::Item::Oneof::Builder builder) override
//     {
//         // the template argument will dispatch and retrieve different method pointer
//         // depending on the data type
//
//         if (this->has_orphan_getter())
//         {
//             (builder.*NdArrayAdoptOrphanDispatcher<DataType>())(std::move(this->orphan_));
//             spdlog::error("nope");
//         } else {
//             std::vector<DataType> values = {1323,2.1,12312.12312222223};
////            assign_builder(builder);
//            msgx::helpers::build_capnp_list<DataType>(
//                builder.initFloatArray().initBuffer(values.size()), values);
//        }
//    }
//};
//
//}  // namespace conversion
//
//}  // namespace msgx