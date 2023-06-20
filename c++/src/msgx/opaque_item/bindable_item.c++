#include "msgx/opaque_item/bindable_item.h"

#include "msgx/conversion/common.h"
#include "msgx/dispatcher/builder_adopt_dispatcher.h"

namespace msgx
{

void BindableOpaqueItem::build(OpaqueItemBuilder builder)
{
    using msgx::type::Item;

    if (orphan_ != nullptr)
    {
        detail::builder_adopt_orphan(builder, std::move(orphan_));
        orphan_ = {};
        return;
    }
    if (assign_primitive_callback_)
    {
        assign_primitive_callback_(builder);
        return;
    }
}

OpaqueItemBuilder BindableOpaqueItem::get_orphan_or_malloc_builder()
{
    if (!has_orphanage())
        throw std::runtime_error("This ptr is not associated with any orphanage");

    orphan_ = std::move(get_orphanage_functor_().newOrphan<msgx::type::Item::Oneof>());
    return orphan_.get();
}

void BindableOpaqueItem::set_assignment_callback(
    std::function<void(OpaqueItemBuilder builder)> &&assign_primitive_callback)
{
    // should we clear the orphan here? to allow reassignment from primitive to ptr type.

    // this can assign value in-place. Good for primitive values
    assign_primitive_callback_ = std::move(assign_primitive_callback);
}

void BindableOpaqueItem::assign_orphan(capnp::Orphan<type::Item::Oneof> &&orphan)
{
    orphan_ = std::move(orphan);
}

}  // namespace msgx