#pragma once

#include "msgx/conversion/convertible.h"
#include "msgx/opaque_item/mappings.h"

namespace msgx
{
using OpaqueItemList = std::vector<OpaqueItemPtr>;

namespace detail
{

template <typename T,
          typename std::enable_if<!std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value &&  //
                                  !std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
void _anylist_impl(const OrphanageGetter &orphanage_getter, OpaqueItemList &container, T arg)
{
    auto ptr = std::make_unique<BindableOpaqueItem>(orphanage_getter);
    ::msgx::detail::call_conversion<typename std::decay<T>::type>(*ptr, std::move(arg));
    container.push_back(std::move(ptr));
}

template <typename T, typename std::enable_if<std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value ||  //
                                              std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
void _anylist_impl(const OrphanageGetter &orphanage_getter, OpaqueItemList &container, T arg)
{
    (void)orphanage_getter;
    // directly pass-through opaque unique ptr
    container.push_back(std::move(arg));
}

template <typename T, typename... Args>
void _anylist_impl(const OrphanageGetter &orphanage_getter, OpaqueItemList &container, T arg, Args &&...args)
{
    // process one
    _anylist_impl(orphanage_getter, container, std::forward<T>(arg));
    // process the rest
    _anylist_impl(orphanage_getter, container, std::forward<Args>(args)...);
}

}  // namespace detail

// build a any list from a list of opaque items
OrphanOpaqueItem AnyList(OrphanageGetter orphanage_getter, OpaqueItemList &container);

// alias easy function
template <typename... Args,
          typename std::enable_if<
              // only enable if the first item in Args parameter pack is not already an OpaqueItemList
              !std::is_same<std::tuple_element_t<0, std::tuple<Args...>>, OpaqueItemList>::value>::type * = nullptr>
OrphanOpaqueItem AnyList(OrphanageGetter orphanage_getter, Args &&...args)
{
    OpaqueItemList container;
    ::msgx::detail::_anylist_impl(orphanage_getter, container, std::forward<Args>(args)...);

    return AnyList(orphanage_getter, container);
}

}  // namespace msgx
