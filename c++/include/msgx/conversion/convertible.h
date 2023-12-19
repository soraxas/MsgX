#pragma once

#include <cstdint>
#include <iostream>
#include <type_traits>

#include "msgx/opaque_item/bindable_item.h"

namespace msgx
{

template <typename T, typename T2 = void>
struct conversion;

namespace conversion_helper
{
/*
 * trait class to detect if user had added the following class member variable
 * > static constexpr bool directly_assignable = true;
 * to the conversion class, to denote that the value should be directly copied
 * to the call back and assign (instead of allocating an orphan).
 */
template <typename T>
class is_directly_assignable
{
    typedef char yes_type;
    typedef long no_type;
    // the following will be instantiated iff 'conversion::directly_assignable' exists and has a value of true
    template <typename U, typename = typename std::enable_if<U::directly_assignable>::type>
    static yes_type test(decltype(&U::directly_assignable));
    // if the member doesn't exist or is set to false, it will fall to the following case
    template <typename U>
    static no_type test(...);

public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};
}  // namespace conversion_helper

namespace detail
{

/*
 * Conversion for primitives, where we copy value into callback, and directly assign them
 */
template <
    typename T1, typename T2, typename ConversionClass = ::msgx::conversion<typename std::decay<T1>::type>,
    typename std::enable_if<conversion_helper::is_directly_assignable<ConversionClass>::value, void>::type * = nullptr>
void call_conversion(msgx::BindableOpaqueItem &opaque_item, T2 &&other)
{
    opaque_item.set_assignment_callback(
        [other](OpaqueItemBuilder builder)
        {
            // copy values into the callback
            ::msgx::conversion<typename std::decay<T1>::type>::convert(builder, other);
        });
}

/*
 * Conversion for primitives, where we allocate orphan, and then swap the pointer
 */
template <
    typename T1, typename T2, typename ConversionClass = ::msgx::conversion<typename std::decay<T1>::type>,
    typename std::enable_if<!conversion_helper::is_directly_assignable<ConversionClass>::value, void>::type * = nullptr>
void call_conversion(msgx::BindableOpaqueItem &opaque_item, T2 &&other)
{
    if (!opaque_item.has_orphanage())
        throw std::runtime_error("call_conversion currently only support ptr with orphanage");

    SPDLOG_TRACE("creating an orphan for datatype: {}", EASYSPDLOG_TYPE_NAME(T1));
    auto builder = opaque_item.get_orphan_or_malloc_builder();

    // construct it in-place right now to avoid copy
    ::msgx::conversion<typename std::decay<T1>::type>::convert(builder, std::move(std::forward<T2>(other)));
}

}  // namespace detail

}  // namespace msgx