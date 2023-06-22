#pragma once

#include "msgx/conversion/convertible.h"
#include "msgx/opaque_item/mappings.h"

namespace msgx
{
namespace detail
{

template <typename T,
          typename std::enable_if<!std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value &&  //
                                  !std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
void _anylist_impl(const OrphanageGetter &orphanage_getter, std::vector<OpaqueItemPtr> &container, T arg)
{
    auto ptr = std::make_unique<BindableOpaqueItem>(orphanage_getter);
    ::msgx::detail::call_conversion<typename std::decay<T>::type>(*ptr, std::move(arg));
    container.push_back(std::move(ptr));
}

template <typename T, typename std::enable_if<std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value ||  //
                                              std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
void _anylist_impl(const OrphanageGetter &orphanage_getter, std::vector<OpaqueItemPtr> &container, T arg)
{
    // directly pass-through opaque unique ptr
    container.push_back(std::move(arg));
}

template <typename T, typename... Args>
void _anylist_impl(const OrphanageGetter &orphanage_getter, std::vector<OpaqueItemPtr> &container, T arg,
                   Args &&...args)
{
    // process one
    _anylist_impl(orphanage_getter, container, std::forward<T>(arg));
    // process the rest
    _anylist_impl(orphanage_getter, container, std::forward<Args>(args)...);
}

}  // namespace detail

// alias easy function
template <typename... Args>
auto AnyList(OrphanageGetter orphanage_getter, Args &&...args)
{
    std::vector<OpaqueItemPtr> container;
    ::msgx::detail::_anylist_impl(orphanage_getter, container, std::forward<Args>(args)...);

    auto item_orphan = orphanage_getter().newOrphan<msgx::type::Item::Oneof>();
    auto anylist_builder = item_orphan.get().initAnyList(container.size());
    //
    for (size_t i = 0; i < container.size(); ++i)
    {
        auto orphan = orphanage_getter().newOrphan<msgx::type::Item>();

        container[i]->build(orphan.get().initOneof());
        anylist_builder.adoptWithCaveats(i, std::move(orphan));
    }

    return std::move(item_orphan);
}

}  // namespace msgx
