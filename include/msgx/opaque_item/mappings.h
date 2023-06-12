#pragma once

#include <functional>
#include <vector>

#include "msgx.capnp.h"
#include "msgx/opaque_item/composed_items.h"
#include "msgx/opaque_item/conversion.h"
#include "msgx/opaque_item/items.h"

namespace msgx
{

using OpaqueMappingCapnpType = ::msgx::type::Map<::msgx::type::Item, ::msgx::type::Item>;

class OpaqueMapping : public OpaqueComposedItem<OpaqueMappingCapnpType>
{
    using CapnpMsgType = OpaqueMappingCapnpType;

    // use superclass constructor
    using OpaqueComposedItem::OpaqueComposedItem;

    struct IndexAccessProxy
    {
        /*
         * This proxy is returned when accessing fig or trace. This proxy object
         * will in-turn forward the assignment when user assign value to this proxy.
         */

        explicit IndexAccessProxy(const std::function<void(const OpaqueItemPtr value_ptr)> &assignment_callback,
                                  OpaqueMapping *parent);

        template <typename T>
        IndexAccessProxy &operator=(std::initializer_list<T> other)
        {
            //            operator=(other);
            assignment_callback_(::msgx::conversion::opaque_item(mapping_parent_->get_orphanage_functor_, other));
            return *this;
        }

        template <typename T>
        IndexAccessProxy &operator=(T &other)
        {
            auto ptr = std::make_shared<msgx::BAHAHACompltelyOpaqueOitem>(mapping_parent_->get_orphanage_functor_);
            assignment_callback_(::msgx::conversion::opaque_item(ptr, std::forward<T>(other)));
            return *this;
        }

        // r-value
        template <typename T>
        IndexAccessProxy &operator=(T &&other)
        {
            operator=(other);
            return *this;
        }

        IndexAccessProxy operator[](const std::string &key);

    protected:
        const std::function<void(const OpaqueItemPtr value_ptr)> assignment_callback_;
        OpaqueMapping *mapping_parent_;
    };

public:
    void build(msgx::type::Item::Oneof::Builder builder) override;

    IndexAccessProxy operator[](const std::string &key);

    void assign_pair(const std::string &key, OpaqueItemPtr value);

private:
    std::unordered_map<std::string, OpaqueItemPtr> mapping_pair;
};

}  // namespace msgx
