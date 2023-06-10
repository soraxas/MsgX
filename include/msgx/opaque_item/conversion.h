#pragma once

#include <soraxas_toolbox/metaprogramming.h>

#include "msgx/opaque_item/composed_items.h"
#include "msgx/opaque_item/items.h"
#include "msgx/opaque_item/primitive_items.h"

namespace msgx
{
namespace conversion
{

// template <class T, typename = typename std::enable_if<std::is_same<T, bool>::value>::type>
// ItemValuePtr get_ptttt(const T value)
//{
//     return std::make_shared<ItemValueBool>(value);
// }
//
// inline ItemValuePtr get_ptttt(const std::string &value)
//{
//     return std::make_shared<ItemValueString>(value);
// }

// template <class T, typename = typename std::enable_if<std::is_same<T, bool>::value>::type>
// OpaqueItemPtr opaque_item(const T value)
//{
//     return std::make_shared<Bool>(value);
// }
//
// OpaqueItemPtr opaque_item(const std::string &value)
//{
//     return std::make_shared<String>(value);
// }
//

template <typename T>
using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

// identity function that passthrough its input
template <typename Derived, typename = std::enable_if_t<std::is_base_of<::msgx::OpaqueItem, Derived>::value>>
inline OpaqueItemPtr opaque_item(const OrphanageGetter &_, std::shared_ptr<Derived> item)
{
    SPDLOG_DEBUG("[Conversion] passthrough opaque ptr");
    return item;
}

inline OpaqueItemPtr opaque_item(const OrphanageGetter &_, std::nullptr_t)
{
    SPDLOG_DEBUG("[Conversion] nullptr");
    return std::make_shared<Null>();
}

inline OpaqueItemPtr opaque_item(const OrphanageGetter &_, const std::string &value)
{
    SPDLOG_DEBUG("[Conversion] std::string '{}'", value);
    return std::make_shared<String>(value);
}

inline OpaqueItemPtr opaque_item(const OrphanageGetter &_, const char *value)
{
    SPDLOG_DEBUG("[Conversion] string {}", value);
    return std::make_shared<String>(value);
}

inline OpaqueItemPtr opaque_item(const OrphanageGetter &_, bool value)
{
    SPDLOG_DEBUG("[Conversion] bool {}", value);
    return std::make_shared<Bool>(value);
}

template <typename T>
inline OpaqueItemPtr opaque_item(const OrphanageGetter &getter, const std::vector<T> &value)
{
    SPDLOG_DEBUG("[Conversion] vector with size {}", value.size());
    return std::make_shared<msgx::conversion::OpaqueArray<T>>(getter, value);
}

template <typename T>
inline OpaqueItemPtr opaque_item(const OrphanageGetter &getter, const std::initializer_list<T> &value)
{
    SPDLOG_DEBUG("[Conversion] initializer_list with size {}", value.size());



    return std::make_shared<msgx::conversion::OpaqueArray<T>>(getter, value);
}

// match all
template <class T>
OpaqueItemPtr opaque_item(const OrphanageGetter &_, T v)
{
    static_assert(std::is_same<T, std::false_type>::value, "There is no 'opaque_item' function found that "
                                                           "can convert the given type to an OpaqueItem pointer. "
                                                           "Hint: define a conversion function under the specific"
                                                           "namespace for conversion, i.e., "
                                                           "msgx::conversion::opaque_item(T ...)");

    std::terminate();
}

}  // namespace conversion
}  // namespace msgx