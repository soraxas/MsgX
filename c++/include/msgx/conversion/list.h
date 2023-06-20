#pragma once

#include "msgx/conversion/common.h"
#include "msgx/conversion/convertible.h"
#include "msgx/dispatcher/opaque_dispatcher.h"
#include "msgx/opaque_item/item.h"

namespace msgx
{
namespace conversion_helper
{

template <typename DataType, template <typename, typename...> class ContainerType, typename... Args>
void bind_as_list(OpaqueItemBuilder builder, const ContainerType<DataType, Args...> &values)
{
    SPDLOG_DEBUG("[Conversion] binding type {} as list with size '{}'", typeid(DataType).name(), values.size());

    // calls the templated func
    msgx::helpers::build_capnp_list<DataType>(
        // this line calls the corresponding init array by dispatching with trait class
        (builder.*msgx::detail::InitListDispatcher<DataType>())(values.size()), values);
}

}  // namespace conversion_helper

/**
 * The following is used to convert non-numeric type (aka in practice this will be string)
 **/
template <typename DataType, template <typename, typename...> class ContainerType, typename... Args>
struct conversion<ContainerType<DataType, Args...>,
                  typename std::enable_if<                                                     //
                      !helpers::is_numeric_v<DataType> &&                                      // not numeric
                          !helpers::is_eigen_matrix_type_v<ContainerType<DataType, Args...>>,  // not eigen
                      DataType>::type>
{
    static void convert(OpaqueItemBuilder builder, const ContainerType<DataType, Args...> &values)
    {
        conversion_helper::bind_as_list(builder, values);
    }
};

//////////////////////////////////////////////////////////////////////////////

template <typename T>
struct List
{
    explicit List(const T &values) : stored_data_(std::move(values))
    {
    }
    const T &stored_data_;
};

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

/**
 * If user had wrapped the expression inside msgx::List(...), then we will always
 * evaluate it as a List of some homogeneous type.
 **/
template <typename T>
struct conversion<List<T>>
{
    static void convert(OpaqueItemBuilder builder, const List<T> &list_container)
    {
        conversion_helper::bind_as_list(builder, list_container.stored_data_);
    }
};

}  // namespace msgx