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
        SPDLOG_TRACE("creating an orphan for datatype: {}", EASYSPDLOG_TYPE_NAME(T));
        // construct it in-place right now to avoid copy
        _main_body(opaque_item.get_orphan_or_malloc_builder(), values);
    }
    else
    {
        SPDLOG_TRACE("no orphanage, copying datatype to callback: {}", EASYSPDLOG_TYPE_NAME(T));
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

/* Metaprogramming */

template <class T, class decayT = typename std::decay<T>::type>
constexpr bool is_numeric_v = std::is_arithmetic<decayT>::value && !std::is_same<decayT, bool>::value;

template <typename T>
using enable_if_is_numeric_t = typename std::enable_if<is_numeric_v<T>, T>::type;

template <typename T>
using disable_if_is_numeric_t = typename std::enable_if<!is_numeric_v<T>, T>::type;

// meta class for metaprogramming for detecting eigen matrix

// clang-format off
template <class>
struct sfinae_true : std::true_type {};

namespace detail {
    template <class T>
    static auto test_is_matrix_type(int) -> sfinae_true<decltype(std::declval<T>().matrix())>;

    template <class>
    static auto test_is_matrix_type(long) -> std::false_type;
}

template <class T>
struct is_eigen_matrix_type : decltype(detail::test_is_matrix_type<T>(0)) {};

template <typename T>
constexpr bool is_eigen_matrix_type_v = is_eigen_matrix_type<T>::value;

template <typename T>
using disable_if_is_eigen_matrix_type_t = typename std::enable_if<!is_eigen_matrix_type<T>::value, T>::type;
// clang-format on

////////////////

// namespace detail {
// template <class T>
// static auto test_contains_data_ptr(int) -> sfinae_true<decltype(std::declval<T>().data())>;
//
// template <class>
// static auto test_contains_data_ptr(long) -> std::false_type;
// }
//
// template <class T>
// struct has_data_ptr : decltype(detail::test_contains_data_ptr<T>(0)) {};

namespace detail
{
template <typename T>
auto has_data_ptr_impl(int) -> decltype(  //
    void(*std::declval<T>().data()),      // operator*
    std::true_type{});

template <typename T>
std::false_type has_data_ptr_impl(...);
}  // namespace detail

template <typename T>
using has_data_ptr = decltype(detail::has_data_ptr_impl<T>(0));

template <typename T>
constexpr bool has_data_ptr_v = has_data_ptr<T>::value;

////////////////

}  // namespace helpers
}  // namespace msgx
