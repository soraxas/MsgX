#pragma once

#include <spdlog/spdlog.h>

#include "msgx/def.h"

namespace msgx
{

class OpaqueItem
{
public:
    explicit OpaqueItem(OrphanageGetter get_orphanage_functor = nullptr);

    virtual void build(OpaqueItemBuilder builder) = 0;

    bool has_orphanage() const;

protected:
    const OrphanageGetter get_orphanage_functor_;
};

using OpaqueItemPtr = std::unique_ptr<OpaqueItem>;

}  // namespace msgx