#include "msgx/message.h"

namespace msgx
{

MessageX::MessageX() : orphanage_getter_([this]() { return msg_builder_.getOrphanage(); })
{
}

void MessageX::send()
{
    auto root_builder = msg_builder_.initRoot<msgx::type::Item>();

    switch (root_contained_item_.type)
    {
        case OpaqueMappingOrBindableItem::unionType::Mapping:
            root_contained_item_.mapping_.build(root_builder.initOneof());
            break;
        case OpaqueMappingOrBindableItem::unionType::BindableItem:
            root_contained_item_.bindable_item_.build(root_builder.initOneof());
            break;
        case OpaqueMappingOrBindableItem::unionType::Empty:
            // it's emtpy :L but we will send it eitherway
            SPDLOG_DEBUG("Message to be send is empty");
            break;
    }

    comms::Zmq::Send(msg_builder_);
}

// forward indexing call to inner mapping
auto MessageX::operator[](const std::string &key)
    -> decltype(std::declval<OpaqueMapping>()[std::declval<const std ::string &>()])
{
    // initialise this root as a map
    if (root_contained_item_.type != OpaqueMappingOrBindableItem::unionType::Mapping)
    {
        root_contained_item_.set_mapping([this]() { return msg_builder_.getOrphanage(); });
    }
    return root_contained_item_.mapping_[key];
}

OrphanageGetter MessageX::get_orphanage_getter() const
{
    return orphanage_getter_;
}

}  // namespace msgx