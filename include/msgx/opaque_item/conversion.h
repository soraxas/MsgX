#pragma once

#include <soraxas_toolbox/metaprogramming.h>

#include "msgx/opaque_item/composed_items.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/primitive_items.h"

namespace msgx
{
namespace conversion
{

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, std::nullptr_t)
{
    SPDLOG_DEBUG("[Conversion] nullptr");
    item->set_assignment_callback([](OpaqueItemBuilder builder) { builder.setNull(); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const bool value)
{
    SPDLOG_DEBUG("[Conversion] bool '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setBool(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const std::string &value)
{
    SPDLOG_DEBUG("[Conversion] std::string '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const char *value)
{
    SPDLOG_DEBUG("[Conversion] const char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setString(value); });
    return item;
}

// identity function that passthrough its input
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, std::shared_ptr<Derived> rhs_item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque ptr");
    item->set_assignment_callback([rhs_item](OpaqueItemBuilder builder) { rhs_item->build(builder); });
    return item;
}

/*
 * ===== signed =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setChar(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int16_t value)
{
    SPDLOG_DEBUG("[Conversion] int16_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setShort(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int32_t value)
{
    SPDLOG_DEBUG("[Conversion] int32_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setInt(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const int64_t value)
{
    SPDLOG_DEBUG("[Conversion] int64_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setLong(value); });
    return item;
}

/*
 * ===== unsigned =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const unsigned char value)
{
    SPDLOG_DEBUG("[Conversion] char '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUChar(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint16_t value)
{
    SPDLOG_DEBUG("[Conversion] uint16_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUShort(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint32_t value)
{
    SPDLOG_DEBUG("[Conversion] uint32_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setUInt(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const uint64_t value)
{
    SPDLOG_DEBUG("[Conversion] uint64_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setULong(value); });
    return item;
}

/*
 * ===== floating point =====
 */
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const float_t value)
{
    SPDLOG_DEBUG("[Conversion] float_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setFloat(value); });
    return item;
}

inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const double_t value)
{
    SPDLOG_DEBUG("[Conversion] double_t '{}'", value);
    item->set_assignment_callback([value](OpaqueItemBuilder builder) { builder.setDouble(value); });
    return item;
}

//// match all
// template <class T>
// OpaqueItemPtr opaque_item(const msgx::MyOpaqueItemPtr &item, T v)
//{
//     static_assert(std::is_same<T, std::false_type>::value, "There is no 'opaque_item' function found that "
//                                                            "can convert the given type to an OpaqueItem pointer. "
//                                                            "Hint: define a conversion function under the specific"
//                                                            "namespace for conversion, i.e., "
//                                                            "msgx::conversion::opaque_item(T ...)");
//
//     std::terminate();
// }

// inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const Mapping value)
//{
//     SPDLOG_DEBUG("[Conversion] numeric '{}'", value);
//  ue_item->set_assignment_callback([value] { builder.initOneof().initMapping(); });
// }

/*
    using IterableContainerIteratorType = decltype(std::begin(std::declval<IterableContainer>()));
    // the following does not work if the iterable container did not declare its value type
//    using ValueType = typename std::iterator_traits<IterableContainerIteratorType>::value_type;
    using ValueType = decltype(*std::declval<IterableContainerIteratorType>());
*/

// template <typename Container, typename DataType = typename Container::value_type>
// inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const Container &values)
//{
//     //    using DataType = typename Container::value_type;
//     SPDLOG_DEBUG("[Conversion] vector type {} with size '{}'", typeid(DataType).name(), values.size());
//
//     msgx::helpers::build_capnp_list<DataType>(                           // calls the templated func
//         (builder.initOneof().*NdArrayInitArrayDispatcher<DataType>())()  // this line calls the corresponding init
//         array
//             .initBuffer(values.size()),
//         values);
// }

template <typename T>
void _run_callback_with_or_without_orphanage(const msgx::MyOpaqueItemPtr &opaque_item, const T &values,
                                             const std::function<void(OpaqueItemBuilder, const T &)> _main_body)
{
    if (opaque_item->has_orphanage())
    {
        SPDLOG_TRACE("creating an orphan for datatype: {}", typeid(T).name());
        // construct it in-place right now to avoid copy
        _main_body(opaque_item->get_orphan_or_malloc_builder(), values);
    }
    else
    {
        SPDLOG_TRACE("no orphanage, copying datatype to callback: {}", typeid(T).name());
        // bind a lambda by copying value, and set it as callback
        opaque_item->set_assignment_callback(                //
            [values, _main_body](OpaqueItemBuilder builder)  //
            {                                                //
                _main_body(builder, values);                 //
            });
    }
}

// template <typename DataType>
// inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const std::vector<DataType> &values)
//{
//     SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());
//
//     msgx::helpers::build_capnp_list<DataType>(                           // calls the templated func
//         (builder.initOneof().*NdArrayInitArrayDispatcher<DataType>())()  // this line calls the corresponding init
//         array
//             .initBuffer(values.size()),
//         values);
// }

// template <typename DataType>
// inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const std::initializer_list<DataType> &values)
//{
//     SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());
//
//     msgx::helpers::build_capnp_list<DataType>(                           // calls the templated func
//         (builder.initOneof().*NdArrayInitArrayDispatcher<DataType>())()  // this line calls the corresponding init
//         array
//             .initBuffer(values.size()),
//         values);
// }

template <typename ContainerDataType, typename DataType = typename ContainerDataType::value_type>
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const ContainerDataType &values)
{
    auto _main_body = [](OpaqueItemBuilder builder, const ContainerDataType &values)
    {
        msgx::helpers::build_capnp_list<DataType>(               // calls the templated func
            (builder.*NdArrayInitArrayDispatcher<DataType>())()  // this line calls the corresponding init array
                .initBuffer(values.size()),
            values);
    };

    _run_callback_with_or_without_orphanage<ContainerDataType>(item, values, _main_body);
    return item;
}

template <typename DataType>
inline auto opaque_item(const msgx::MyOpaqueItemPtr &item, const std::initializer_list<DataType> &values)
{
    SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());

    return msgx::conversion::opaque_item<std::initializer_list<DataType>>(item, values);
}

}  // namespace conversion
}  // namespace msgx