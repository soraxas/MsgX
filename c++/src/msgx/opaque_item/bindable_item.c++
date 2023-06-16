#include "msgx/opaque_item/bindable_item.h"

#include "msgx/conversion/common.h"

namespace msgx
{

void BindableOpaqueItem::build(OpaqueItemBuilder builder)
{
    using msgx::type::Item;

    if (orphan_ != nullptr)
    {
        switch (orphan_.get().which())
        {
            case Item::Oneof::NULL_:
            case Item::Oneof::BOOL:
            case Item::Oneof::CHAR:
            case Item::Oneof::SHORT:
            case Item::Oneof::INT:
            case Item::Oneof::LONG:
            case Item::Oneof::U_CHAR:
            case Item::Oneof::U_SHORT:
            case Item::Oneof::U_INT:
            case Item::Oneof::U_LONG:
            case Item::Oneof::FLOAT:
            case Item::Oneof::DOUBLE:
                throw std::runtime_error(helpers::StringStreamer()
                                         << "An orphan is given with primitive type " << orphan_.get().which()
                                         << ". You should directly set the primitive value instead!");
            case Item::Oneof::INT_LIST:
                builder.adoptIntList(orphan_.get().disownIntList());
                break;
            case Item::Oneof::STRING:
                builder.adoptString(orphan_.get().disownString());
                break;
            case Item::Oneof::BOOL_LIST:
                builder.adoptBoolList(orphan_.get().disownBoolList());
                break;
            case Item::Oneof::LONG_LIST:
                builder.adoptLongList(orphan_.get().disownLongList());
                break;
            case Item::Oneof::FLOAT_LIST:
                builder.adoptFloatList(orphan_.get().disownFloatList());
                break;
            case Item::Oneof::DOUBLE_LIST:
                builder.adoptDoubleList(orphan_.get().disownDoubleList());
                break;
            case Item::Oneof::STRING_LIST:
                builder.adoptStringList(orphan_.get().disownStringList());
                break;
            case Item::Oneof::ANY_LIST:
                builder.adoptAnyList(orphan_.get().disownAnyList());
                break;
            case Item::Oneof::MAPPING:
                builder.adoptMapping(orphan_.get().disownMapping());
                break;
            case Item::Oneof::BINARY:
                builder.adoptBinary(orphan_.get().disownBinary());
                break;
            case Item::Oneof::NAMED_BINARY:
                builder.adoptNamedBinary(orphan_.get().disownNamedBinary());
                break;
            case Item::Oneof::ND_ARRAY:
                builder.adoptNdArray(orphan_.get().disownNdArray());
                break;
            default:
                SPDLOG_ERROR("unimplemented message for which enum: {}", orphan_.get().which());
                throw std::runtime_error("unimplemented message type! ");
        }
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