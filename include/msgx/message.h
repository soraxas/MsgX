#pragma once

#include <capnp/message.h>

#include <utility>

#include "msgx/comms/zmq.h"
#include "msgx/def.h"
#include "msgx/opaque_item/mappings.h"

class OpaqueMappingOrBindableItem
{
public:
    constexpr OpaqueMappingOrBindableItem() : uninitialised_dummy_{false}
    {
    }

    ~OpaqueMappingOrBindableItem()
    {
        destroy();
    }

    void destroy()
    {
        switch (type)
        {
            case unionType::Empty:
                break;
            case unionType::BindableItem:
                mapping_.~OpaqueMapping();
                break;
            case unionType::Mapping:
                bindable_item_.~BindableOpaqueItem();
                break;
        }
        type = unionType::Empty;
    }

    void set_mapping(msgx::OrphanageGetter get_orphanage_functor)
    {
        destroy();
        new (&mapping_) msgx::OpaqueMapping{std::move(get_orphanage_functor)};
        type = unionType::Mapping;
    }

    void set_bindable_item(msgx::OrphanageGetter get_orphanage_functor)
    {
        destroy();
        new (&bindable_item_) msgx::BindableOpaqueItem{std::move(get_orphanage_functor)};
        type = unionType::BindableItem;
    }

    enum class unionType
    {
        Empty,
        BindableItem,
        Mapping
    } type = unionType::Empty;  // match the default constructor of enum

    union
    {
        bool uninitialised_dummy_;
        msgx::OpaqueMapping mapping_;
        msgx::BindableOpaqueItem bindable_item_;
    };
};

namespace msgx
{
class MessageX
{
public:
    MessageX() : orphanage_getter_([this]() { return msg_builder_.getOrphanage(); })
    {
    }

    template <class... Args>
    OpaqueItemPtr container(Args &&...args)
    {
        auto ptr = std::make_unique<msgx::BindableOpaqueItem>(orphanage_getter_);
        ::msgx::conversion::opaque_item(*ptr, std::forward<Args>(args)...);
        return ptr;
    }

    template <typename T>
    MessageX &operator=(T &other)
    {
        contained_item_.set_bindable_item(get_orphanage_getter());

        ::msgx::conversion::opaque_item(contained_item_.bindable_item_, std::forward<T>(other));
        return *this;
    }

    template <typename T>
    MessageX &operator=(const std::initializer_list<T> &other)
    {
        return operator=<const std::initializer_list<T>>(other);  // NOLINT(misc-unconventional-assign-operator)
    }

    // r-value
    template <typename T>
    MessageX &operator=(T &&other)
    {
        return operator=(other);  // NOLINT(misc-unconventional-assign-operator)
    }

    void send()
    {
        auto root_builder = msg_builder_.initRoot<msgx::type::Item>();

        switch (contained_item_.type)
        {
            case OpaqueMappingOrBindableItem::unionType::Mapping:
                contained_item_.mapping_.build(root_builder.initOneof());
                break;
            case OpaqueMappingOrBindableItem::unionType::BindableItem:
                contained_item_.bindable_item_.build(root_builder.initOneof());
                break;
            case OpaqueMappingOrBindableItem::unionType::Empty:
                // it's emtpy :L but we will send it eitherway
                SPDLOG_DEBUG("Message to be send is empty");
                break;
        }

        comms::Zmq::Send(msg_builder_);
    }

    // forward indexing call to inner mapping
    auto operator[](const std::string &key)
    {
        // initialise this root as a map
        if (contained_item_.type != OpaqueMappingOrBindableItem::unionType::Mapping)
        {
            contained_item_.set_mapping([this]() { return msg_builder_.getOrphanage(); });
        }
        return contained_item_.mapping_[key];
    }

    template <typename OpaqueItemType>
    std::shared_ptr<OpaqueItemType> getLinkedItemPtr()
    {
        return std::make_unique<OpaqueItemType>([this]() { return msg_builder_.getOrphanage(); });
    }

    // alias easy function
    template <typename... Args>
    auto Mapping(Args... args)
    {
        return getLinkedItemPtr<OpaqueMapping>();
    }

    OrphanageGetter get_orphanage_getter() const
    {
        return orphanage_getter_;
    }

protected:
    capnp::MallocMessageBuilder msg_builder_;

    OrphanageGetter orphanage_getter_;

    OpaqueMappingOrBindableItem contained_item_;
};

}  // namespace msgx