#include "msgx/opaque_item/item.h"

namespace msgx
{

OpaqueItem::OpaqueItem(OrphanageGetter get_orphanage_functor) : get_orphanage_functor_(std::move(get_orphanage_functor))
{
}

bool OpaqueItem::has_orphanage() const
{
    return get_orphanage_functor_ != nullptr;
}

}  // namespace msgx