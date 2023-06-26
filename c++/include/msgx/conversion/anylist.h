#pragma once

#include "msgx/conversion/common.h"
#include "msgx/conversion/convertible.h"

namespace msgx
{

namespace detail
{
// To allow ADL with custom begin/end
using std::begin;
using std::end;

template <typename T>
auto is_iterable_impl(int)
    -> decltype(begin(std::declval<T &>()) != end(std::declval<T &>()),    // begin/end and operator !=
                void(),                                                    // Handle evil operator ,
                ++std::declval<decltype(begin(std::declval<T &>())) &>(),  // operator ++
                void(*begin(std::declval<T &>())),                         // operator*
                std::true_type{});

template <typename T>
std::false_type is_iterable_impl(...);

}  // namespace detail

template <typename T>
using is_iterable = decltype(detail::is_iterable_impl<T>(0));

/////////////////////////////////////////////////////////

template <typename ContainerType>
struct conversion<ContainerType,
                  typename std::enable_if<  //
                      !helpers::is_numeric_v<
                          // the following will resolve to the contained element type (through iterator interface)
                          decltype(*std::begin(std::declval<ContainerType>()))>  // is not numeric
                          && is_iterable<ContainerType>::value,
                      void>::type>
{
    static void convert(msgx::OpaqueItemBuilder builder, const ContainerType &values)
    {
        using DataType = typename std::decay<decltype(*std::begin(std::declval<ContainerType>()))>::type;

        auto anylist_builder = builder.initAnyList(values.size());

        size_t i = 0;
        for (auto &&val : values)
        {
            msgx::conversion<DataType>::convert(anylist_builder[i].initOneof(), val);
            ++i;
        }
    }
};

}  // namespace msgx
