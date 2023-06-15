#pragma once

#include "msgx/conversion/common.h"
#include "msgx/opaque_item/item.h"
#include "msgx/opaque_item/opaque_dispatcher.h"

namespace msgx
{
namespace conversion
{

template <typename ContainerDataType, typename DataType = typename ContainerDataType::value_type,
          typename helpers::disable_if_is_numeric_t<DataType> * = nullptr>
void opaque_item(msgx::BindableOpaqueItem &item, const ContainerDataType &values)
{
    auto _main_body = [](OpaqueItemBuilder builder, const ContainerDataType &values)
    {
        msgx::helpers::build_capnp_list<DataType>(                                   // calls the templated func
            (builder.*msgx::detail::InitListDispatcher<DataType>())(values.size()),  // this line calls the
                                                                                     // corresponding init array
            values);
    };

    ::msgx::helpers::_run_callback_with_or_without_orphanage<ContainerDataType>(item, values, _main_body);
}

template <typename DataType, typename helpers::disable_if_is_numeric_t<DataType> * = nullptr>
void opaque_item(msgx::BindableOpaqueItem &item, const std::initializer_list<DataType> &values)
{
    SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());

    msgx::conversion::opaque_item<std::initializer_list<DataType>>(item, values);
}

}  // namespace conversion
}  // namespace msgx