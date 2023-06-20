#pragma once

#include <capnp/message.h>

#include <utility>

#include "msgx/comms/zmq.h"
#include "msgx/conversion/anylist_interface.h"
#include "msgx/conversion/mapping_interface.h"
#include "msgx/def.h"
#include "msgx/kwargs.h"
#include "msgx/opaque_item/opaque_union.h"

namespace msgx
{
class MessageX
{
public:
    explicit MessageX();

    // non-copyable
    MessageX(const MessageX &) = delete;

    MessageX &operator=(const MessageX &) = delete;

    //    template <class... Args>
    //    OpaqueItemPtr OpaqueItem(Args &&...args)
    //    {
    //        auto ptr = std::make_unique<msgx::BindableOpaqueItem>(orphanage_getter_);
    //        ::msgx::conversion_helper::opaque_item(*ptr, std::forward<Args>(args)...);
    //        return std::move(ptr);
    //    }

    template <typename T>
    MessageX &operator=(T &other)
    {
        // convert this message to a unit item
        root_contained_item_.set_bindable_item(get_orphanage_getter());

        ::msgx::detail::call_conversion<typename std::decay<T>::type>(root_contained_item_.bindable_item_, other);
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

    void send();

    // forward indexing call to inner mapping
    auto operator[](const std::string &key)
        -> decltype(std::declval<OpaqueMapping>()[std::declval<const std ::string &>()]);

    template <typename OpaqueItemType>
    std::unique_ptr<OpaqueItemType> getLinkedItemPtr()
    {
        return std::make_unique<OpaqueItemType>([this]() { return msg_builder_.getOrphanage(); });
    }

    // alias easy function
    template <typename... Args>
    auto Mapping(Args... args)
    {
        return std::move(msgx::Mapping(orphanage_getter_, std::forward<Args>(args)...));
    }

    // alias easy function
    template <typename... Args>
    auto AnyList(Args... args)
    {
        return std::move(msgx::AnyList(orphanage_getter_, std::forward<Args>(args)...));
    }

    OrphanageGetter get_orphanage_getter() const;

protected:
    capnp::MallocMessageBuilder msg_builder_;

    OrphanageGetter orphanage_getter_;

    // this message is either a map (dictionary) or a unit opaque item
    OpaqueMappingOrBindableItem root_contained_item_;
};

}  // namespace msgx