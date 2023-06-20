#include "msgx/dispatcher/builder_adopt_dispatcher.h"

namespace msgx
{
namespace detail
{

void builder_adopt_orphan(OpaqueItemBuilder builder, capnp::Orphan<type::Item::Oneof> orphan)
{
    using msgx::type::Item;

    switch (orphan.get().which())
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
                                     << "An orphan is given with primitive type " << orphan.get().which()
                                     << ". You should directly set the primitive value instead!");
        case Item::Oneof::INT_LIST:
            builder.adoptIntList(orphan.get().disownIntList());
            break;
        case Item::Oneof::STRING:
            builder.adoptString(orphan.get().disownString());
            break;
        case Item::Oneof::BOOL_LIST:
            builder.adoptBoolList(orphan.get().disownBoolList());
            break;
        case Item::Oneof::LONG_LIST:
            builder.adoptLongList(orphan.get().disownLongList());
            break;
        case Item::Oneof::FLOAT_LIST:
            builder.adoptFloatList(orphan.get().disownFloatList());
            break;
        case Item::Oneof::DOUBLE_LIST:
            builder.adoptDoubleList(orphan.get().disownDoubleList());
            break;
        case Item::Oneof::STRING_LIST:
            builder.adoptStringList(orphan.get().disownStringList());
            break;
        case Item::Oneof::ANY_LIST:
            builder.adoptAnyList(orphan.get().disownAnyList());
            break;
        case Item::Oneof::MAPPING:
            builder.adoptMapping(orphan.get().disownMapping());
            break;
        case Item::Oneof::BINARY:
            builder.adoptBinary(orphan.get().disownBinary());
            break;
        case Item::Oneof::NAMED_BINARY:
            builder.adoptNamedBinary(orphan.get().disownNamedBinary());
            break;
        case Item::Oneof::ND_ARRAY:
            builder.adoptNdArray(orphan.get().disownNdArray());
            break;
        default:
            SPDLOG_ERROR("unimplemented message for which enum: {}", orphan.get().which());
            throw std::runtime_error("unimplemented message type! ");
    }
}

}  // namespace detail
}  // namespace msgx