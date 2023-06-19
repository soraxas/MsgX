#pragma once

#include "msgx/conversion/common.h"
#include "msgx/opaque_item/item.h"
#include "msgx/opaque_item/opaque_dispatcher.h"

namespace msgx
{
namespace conversion
{

template <typename DataType, template <typename, typename...> class ContainerType, typename... Args>
void _bind_as_list(msgx::BindableOpaqueItem &item, const ContainerType<DataType, Args...> &values)
{
    using Container = ContainerType<DataType, Args...>;
    SPDLOG_DEBUG("[Conversion] binding type {} as list with size '{}'", typeid(DataType).name(), values.size());

    auto _main_body = [](OpaqueItemBuilder builder, const Container &values)
    {
        msgx::helpers::build_capnp_list<DataType>(                                   // calls the templated func
            (builder.*msgx::detail::InitListDispatcher<DataType>())(values.size()),  // this line calls the
                                                                                     // corresponding init array
            values);
    };

    ::msgx::helpers::_run_callback_with_or_without_orphanage<Container>(item, values, _main_body);
}

// template <typename ContainerDataType, typename DataType = typename ContainerDataType::value_type,
//           typename helpers::disable_if_is_numeric_t<DataType> * = nullptr>

template <typename DataType, template <typename, typename...> class ContainerType, typename... Args,
          typename helpers::disable_if_is_numeric_t<DataType> * = nullptr,
          typename helpers::disable_if_is_eigen_matrix_type_t<ContainerType<DataType, Args...>> * = nullptr>
void opaque_item(msgx::BindableOpaqueItem &item, const ContainerType<DataType, Args...> &values)
{
    _bind_as_list(item, values);
}

// template <typename DataType, typename helpers::disable_if_is_numeric_t<DataType> * = nullptr>
// void opaque_item(msgx::BindableOpaqueItem &item, const std::initializer_list<DataType> &values)
//{
//     msgx::conversion::opaque_item<std::initializer_list<DataType>>(item, values);
// }

template <typename T>
struct List
{
    explicit List(const T &values) : stored_data_(std::move(values))
    {
    }
    const T &stored_data_;
};

template <typename T>
void opaque_item(msgx::BindableOpaqueItem &item, const List<T> &list_container)
{
    _bind_as_list(item, list_container.stored_data_);
}

template <typename T>
auto AsList(const std::initializer_list<T> &values)
{
    return List<std::initializer_list<T>>(values);
}

template <typename T>
auto AsList(const T &values)
{
    return List<T>(values);
}

}  // namespace conversion
}  // namespace msgx