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

template <typename DataType, template <typename, typename...> class ContainerType, typename... Args>
struct conversion<ContainerType<DataType, Args...>,
                  typename std::enable_if<!helpers::is_numeric_v<DataType>  // is not numeric
                                              && is_iterable<ContainerType<DataType, Args...>>::value,
                                          void>::type>
{
    static void convert(msgx::OpaqueItemBuilder builder, const ContainerType<DataType, Args...> &values)
    {
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
