#pragma once

#include <capnp/message.h>

#include "msgx/comms/zmq.h"
#include "msgx/def.h"
#include "msgx/opaque_item/mappings.h"

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
        auto ptr = std::make_shared<msgx::BAHAHACompltelyOpaqueOitem>(orphanage_getter_);
        return ::msgx::conversion::opaque_item(ptr, std::forward<Args>(args)...);
    }

    template <typename T>
    MessageX &operator=(T &other)
    {
        _destroy_maybe_mapping();
        //        /auto root_builder = msg_builder_.initRoot<msgx::type::Item>();
        maybe_opaque_item_ = std::make_shared<msgx::BAHAHACompltelyOpaqueOitem>(get_orphanage_getter());

        ::msgx::conversion::opaque_item(maybe_opaque_item_, std::forward<T>(other));
        return *this;
    }

    template <typename T>
    MessageX &operator=(const std::initializer_list<T> &other)
    {
        // explicitly forward initialiser list
        _destroy_maybe_mapping();
        maybe_opaque_item_ = std::make_shared<msgx::BAHAHACompltelyOpaqueOitem>(get_orphanage_getter());

        ::msgx::conversion::opaque_item(maybe_opaque_item_, other);
        return *this;
    }

    // r-value
    template <typename T>
    MessageX &operator=(T &&other)
    {
        operator=(other);
        return *this;
    }

    //    MessageX &operator=(OpaqueItemPtr &other)
    //    {
    //        ::msgx::conversion::opaque_item(root_builder, other);
    //        return *this;
    //    }

    void send()
    {
        if (maybe_mapping_)
        {
            auto root_builder = msg_builder_.initRoot<msgx::type::Item>();
            maybe_mapping_->build(root_builder.initOneof());
            _destroy_maybe_mapping();
        }
        else if (maybe_opaque_item_)
        {
            auto root_builder = msg_builder_.initRoot<msgx::type::Item>();
            maybe_opaque_item_->build(root_builder.initOneof());
        }
        comms::Zmq::Send(msg_builder_);
    }

    void _destroy_maybe_mapping()
    {
        maybe_mapping_.reset();
    }

    // forward indexing call to inner mapping
    auto operator[](const std::string &key)
    {
        // initialise this root as a map
        if (!maybe_mapping_)
        {
            maybe_mapping_ = std::make_unique<OpaqueMapping>([this]() { return msg_builder_.getOrphanage(); });
        }
        return (*maybe_mapping_)[key];
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
public:
    capnp::MallocMessageBuilder msg_builder_;

    OrphanageGetter orphanage_getter_;

    std::unique_ptr<OpaqueMapping> maybe_mapping_;

    std::shared_ptr<msgx::BAHAHACompltelyOpaqueOitem> maybe_opaque_item_;

    //    OpaqueItemPtr root_;

public:
};

}  // namespace msgx