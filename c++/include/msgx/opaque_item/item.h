#pragma once

#include <spdlog/spdlog.h>

#include "msgx/def.h"

namespace msgx
{

class OpaqueItem
{
public:
    explicit OpaqueItem(OrphanageGetter get_orphanage_functor = nullptr);

    // non-copyable
    OpaqueItem(const OpaqueItem &) = delete;

    OpaqueItem &operator=(const OpaqueItem &) = delete;

    // move constructor
    OpaqueItem(OpaqueItem &&other) noexcept : get_orphanage_functor_(other.get_orphanage_functor_)
    {
    }

    virtual void build(OpaqueItemBuilder builder) = 0;

    bool has_orphanage() const;

protected:
    const OrphanageGetter get_orphanage_functor_;
};

using OpaqueItemPtr = std::unique_ptr<OpaqueItem>;
using OpaqueItemSharedPtr = std::shared_ptr<OpaqueItem>;

}  // namespace msgx