#pragma once

#include <sstream>
#include <vector>

#include "msgx.capnp.h"
#include "msgx/device_specifics.h"
#include "msgx/opaque_item/bindable_item.h"

namespace msgx
{
namespace helpers
{

/*
 * A helper class that can be used to stream content inside and output
 * std::string
 */
class StringStreamer
{
public:
    template <typename T>
    StringStreamer &operator<<(T stuff)
    {
        ss << stuff;
        return *this;
    }
    operator std::string() const
    {
        return ss.str();
    }

private:
    std::stringstream ss;
};

template <typename T>
void _run_callback_with_or_without_orphanage(msgx::BindableOpaqueItem &opaque_item, const T &values,
                                             const std::function<void(OpaqueItemBuilder, const T &)> _main_body)
{
    if (opaque_item.has_orphanage())
    {
        SPDLOG_TRACE("creating an orphan for datatype: {}", typeid(T).name());
        // construct it in-place right now to avoid copy
        _main_body(opaque_item.get_orphan_or_malloc_builder(), values);
    }
    else
    {
        SPDLOG_TRACE("no orphanage, copying datatype to callback: {}", typeid(T).name());
        // bind a lambda by copying value, and set it as callback
        opaque_item.set_assignment_callback(                 //
            [values, _main_body](OpaqueItemBuilder builder)  //
            {                                                //
                _main_body(builder, values);                 //
            });
    }
}

template <typename T, typename ListReader>
std::vector<T> get_capnp_list(ListReader reader)
{
    static_assert(sizeof(T) == sizeof(decltype(*reader.begin())), "size mismatch");
    std::vector<T> container(reader.size());
    std::copy(reader.begin(), reader.end(), container.begin());
    return container;
}

template <typename T, typename ListBuilder>
ListBuilder build_capnp_list(ListBuilder builder, const std::vector<T> &values)
// auto build_capnp_list(typename capnp::List<T>::Builder builder, const std::vector<U> &values)
{
    if (builder.size() != values.size())
        throw std::runtime_error(StringStreamer() << "CapnpBuilder has size " << builder.size()
                                                  << ", but input list has size " << values.size());

    for (size_t i = 0; i < values.size(); ++i)
        builder.set(i, values[i]);
    return builder;
}

constexpr auto get_device_specific_endian()
{
    if (::msgx::helpers::Endian::little)
        return ::msgx::type::ndarray::Endianness::LITTLE_ENDIAN_ORDER;
    else if (::msgx::helpers::Endian::big)
        return ::msgx::type::ndarray::Endianness::BIG_ENDIAN_ORDER;
    else if (::msgx::helpers::Endian::middle)
        return ::msgx::type::ndarray::Endianness::BIG_ENDIAN_ORDER;
}

template <typename T>
using enable_if_is_numeric_t = typename std::enable_if<std::is_arithmetic<T>::value, T>::type;

template <typename T>
using disable_if_is_numeric_t = typename std::enable_if<!std::is_arithmetic<T>::value, T>::type;

}  // namespace helpers
}  // namespace msgx
