#pragma once

#include <capnp/message.h>

#include <utility>

#include "msgx/comms/zmq.h"
#include "msgx/def.h"
#include "msgx/kwargs.h"
#include "msgx/opaque_item/opaque_union.h"

namespace msgx
{
class MessageX
{
public:
    MessageX() : orphanage_getter_([this]() { return msg_builder_.getOrphanage(); })
    {
    }

    template <class... Args>
    OpaqueItemPtr OpaqueItem(Args &&...args)
    {
        auto ptr = std::make_unique<msgx::BindableOpaqueItem>(orphanage_getter_);
        ::msgx::conversion::opaque_item(*ptr, std::forward<Args>(args)...);
        return std::move(ptr);
    }

    template <typename T>
    MessageX &operator=(T &other)
    {
        // convert this message to a unit item
        root_contained_item_.set_bindable_item(get_orphanage_getter());

        ::msgx::conversion::opaque_item(root_contained_item_.bindable_item_, std::forward<T>(other));
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
    auto operator[](const std::string &key)
    {
        // initialise this root as a map
        if (root_contained_item_.type != OpaqueMappingOrBindableItem::unionType::Mapping)
        {
            root_contained_item_.set_mapping([this]() { return msg_builder_.getOrphanage(); });
        }
        return root_contained_item_.mapping_[key];
    }

    template <typename OpaqueItemType>
    std::unique_ptr<OpaqueItemType> getLinkedItemPtr()
    {
        return std::make_unique<OpaqueItemType>([this]() { return msg_builder_.getOrphanage(); });
    }

    // alias easy function
    template <typename... Args>
    auto Mapping(Args... args)
    {
        auto opaque_mapping_ptr = getLinkedItemPtr<OpaqueMapping>();

        __Mapping(*opaque_mapping_ptr, std::forward<Args>(args)...);
        return opaque_mapping_ptr;
    }

    auto __Mapping(OpaqueMapping &mapping)
    {
    }

    auto __Mapping(OpaqueMapping &mapping, Kwargs &kwarg)
    {
        // return getLinkedItemPtr<OpaqueMapping>();  // FIXME
        //        mapping[kwarg.key_] = kwarg.

        auto opaque_item = getLinkedItemPtr<msgx::BindableOpaqueItem>();

        kwarg.build_functor_(*opaque_item);
        mapping.assign_pair(kwarg.key_, std::move(opaque_item));
    }

    template <typename... Args>
    auto __Mapping(OpaqueMapping &mapping, Kwargs &&kwarg, Args... args)
    {
        __Mapping(mapping, kwarg);
        __Mapping(mapping, std::forward<Args>(args)...);
    }

    // alias easy function
    template <typename... Args>
    auto AnyList(Args... args)
    {
        std::vector<OpaqueItemPtr> container;
        __AnyList(container, std::forward<Args>(args)...);

        SPDLOG_TRACE("haha {}", container.size());

        //        auto anyitem_list = orphanage_getter_().newOrphan<capnp::List<msgx::type::Item>>();

        auto item_orphan = orphanage_getter_().newOrphan<msgx::type::Item::Oneof>();
        auto anylist = item_orphan.get().initAnyArray();
        //
        auto anylist_builder = anylist.initBuffer(container.size());
        for (size_t i = 0; i < container.size(); ++i)
        {
            auto orphan = orphanage_getter_().newOrphan<msgx::type::Item>();

            // FIXME???
            container[i]->build(orphan.get().initOneof());

            anylist_builder.adoptWithCaveats(i, std::move(orphan));
            //            anylist_builder.setWithCaveats(i, container[i]->);
        }

        return std::move(item_orphan);
    }

    // alias easy function
    template <typename T, typename... Args>
    void __AnyList(std::vector<OpaqueItemPtr> &container, T arg, Args... args)
    {
        // process one
        __AnyList(container, std::forward<T>(arg));
        // process the rest
        __AnyList(container, std::forward<Args>(args)...);
    }

    template <typename T,
              typename std::enable_if<!std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value &&  //
                                      !std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
    void __AnyList(std::vector<OpaqueItemPtr> &container, T arg)
    {
        auto ptr = getLinkedItemPtr<BindableOpaqueItem>();
        ::msgx::conversion::opaque_item(*ptr, std::move(arg));
        container.push_back(std::move(ptr));
    }

    template <typename T,
              typename std::enable_if<std::is_same<T, std::unique_ptr<BindableOpaqueItem>>::value ||  //
                                      std::is_same<T, std::unique_ptr<OpaqueMapping>>::value>::type * = nullptr>
    void __AnyList(std::vector<OpaqueItemPtr> &container, T arg)
    {
        // directly pass-through opaque unique ptr
        container.push_back(std::move(arg));
    }

    OrphanageGetter get_orphanage_getter() const
    {
        return orphanage_getter_;
    }

protected:
    capnp::MallocMessageBuilder msg_builder_;

    OrphanageGetter orphanage_getter_;

    // this message is either a map (dictionary) or a unit opaque item
    OpaqueMappingOrBindableItem root_contained_item_;
};

}  // namespace msgx