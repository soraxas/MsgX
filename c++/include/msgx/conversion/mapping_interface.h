#pragma once

#include "msgx/kwargs.h"
#include "msgx/opaque_item/mappings.h"

namespace msgx
{
namespace detail
{

inline auto __Mapping(const OrphanageGetter &orphanage_getter, OpaqueMapping &mapping)
{
}

inline auto __Mapping(const OrphanageGetter &orphanage_getter, OpaqueMapping &mapping, Kwargs &kwarg)
{
    auto opaque_item = std::make_unique<BindableOpaqueItem>(orphanage_getter);

    kwarg.build_functor_(*opaque_item);
    mapping.assign_pair(kwarg.key_, std::move(opaque_item));
}

template <typename... Args>
auto __Mapping(const OrphanageGetter &orphanage_getter, OpaqueMapping &mapping, Kwargs &&kwarg, Args &&...args)
{
    __Mapping(orphanage_getter, mapping, kwarg);
    __Mapping(orphanage_getter, mapping, std::forward<Args>(args)...);
}
}  // namespace detail

// alias easy function
template <typename... Args>
auto Mapping(const OrphanageGetter &orphanage_getter, Args &&...args)
{
    auto opaque_mapping_ptr = std::make_unique<OpaqueMapping>(orphanage_getter);

    ::msgx::detail::__Mapping(orphanage_getter, *opaque_mapping_ptr, std::forward<Args>(args)...);
    return std::move(opaque_mapping_ptr);
}
}  // namespace msgx
