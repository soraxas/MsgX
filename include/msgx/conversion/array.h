#pragma once

#include "msgx/conversion/common.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/opaque_dispatcher.h"

namespace msgx
{
namespace conversion
{

template <typename ContainerDataType, typename DataType = typename ContainerDataType::value_type>
void opaque_item(msgx::BindableOpaqueItem &item, const ContainerDataType &values)
{
    auto _main_body = [](OpaqueItemBuilder builder, const ContainerDataType &values)
    {
        msgx::helpers::build_capnp_list<DataType>(               // calls the templated func
            (builder.*NdArrayInitArrayDispatcher<DataType>())()  // this line calls the corresponding init array
                .initBuffer(values.size()),
            values);
    };

    ::msgx::helpers::_run_callback_with_or_without_orphanage<ContainerDataType>(item, values, _main_body);
}

template <typename DataType>
void opaque_item(msgx::BindableOpaqueItem &item, const std::initializer_list<DataType> &values)
{
    SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());

    msgx::conversion::opaque_item<std::initializer_list<DataType>>(item, values);
}

}  // namespace conversion
}  // namespace msgx