#pragma once

#include "msgx/def.h"
#include "msgx/opaque_item/item.h"

namespace msgx
{

class BindableOpaqueItem : public OpaqueItem
{
public:
    using OpaqueItem::OpaqueItem;

    void build(OpaqueItemBuilder builder) override;

    OpaqueItemBuilder get_orphan_or_malloc_builder();

    void set_assignment_callback(std::function<void(OpaqueItemBuilder builder)> &&assign_primitive_callback);

    void assign_orphan(OrphanOpaqueItem &&orphan);

    bool has_orphan() const
    {
        return orphan_ == nullptr;
    }

private:
    OrphanOpaqueItem orphan_;

    std::function<void(OpaqueItemBuilder builder)> assign_primitive_callback_;
};

using BindableOpaqueItemPtr = std::unique_ptr<BindableOpaqueItem>;

}  // namespace msgx