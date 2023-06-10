#pragma once

#include <functional>
#include <vector>

#include "msgx.capnp.h"
#include "msgx/opaque_item/composed_items.h"
#include "msgx/opaque_item/conversion.h"
#include "msgx/opaque_item/items.h"

namespace msgx
{

using OpaqueMappingCapnpType = msgx::Map<msgx::Item, msgx::Item>;

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
        IndexAccessProxy &operator=(T &other);

        template <typename T>
        IndexAccessProxy &operator=(std::initializer_list<T> other)
        {
            //            operator=(other);
            assignment_callback_(::msgx::conversion::opaque_item(mapping_parent_->get_orphange_functor_, other));
            return *this;
        }

        // r-value
        template <typename T>
        IndexAccessProxy &operator=(T &&other);

        template <typename T>
        IndexAccessProxy operator[](const T &key);

    protected:
        const std::function<void(const OpaqueItemPtr value_ptr)> assignment_callback_;
        OpaqueMapping *mapping_parent_;
    };

public:
    void build(msgx::Item::Oneof::Builder builder) override;

    template <typename T>
    IndexAccessProxy operator[](const T &key);

    void assign_pair(OpaqueItemPtr key, OpaqueItemPtr value);

private:
    std::vector<std::array<OpaqueItemPtr, 2>> mapping_pair;
};

}  // namespace msgx
#include "mappings_template.h"
