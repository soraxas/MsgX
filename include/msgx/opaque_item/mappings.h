#pragma once

#include <functional>
#include <unordered_map>

#include "msgx.capnp.h"
#include "msgx/conversion/all.h"
#include "msgx/opaque_item/bindable_item.h"

namespace msgx
{

class OpaqueMapping : public OpaqueItem
{
protected:
    // use superclass constructor
    using OpaqueItem::OpaqueItem;

    struct IndexAccessProxy
    {
        /*
         * This proxy is returned when accessing fig or trace. This proxy object
         * will in-turn forward the assignment when user assign value to this proxy.
         */

        explicit IndexAccessProxy(const std::function<void(const OpaqueItemPtr value_ptr)> &assignment_callback,
                                  OpaqueMapping *parent);

        template <typename T>
        IndexAccessProxy &operator=(T &other)
        {
            auto ptr = std::make_unique<msgx::BindableOpaqueItem>(mapping_parent_->get_orphanage_functor_);
            ::msgx::conversion::opaque_item(*ptr, std::forward<T>(other));
            assignment_callback_(std::move(ptr));
            return *this;
        }

        IndexAccessProxy &operator=(msgx::OpaqueItemPtr other);

        // r-value
        template <typename T>
        IndexAccessProxy &operator=(T &&other)
        {
            return operator=<T>(other);  // NOLINT(misc-unconventional-assign-operator)
        }

        template <typename T>
        IndexAccessProxy &operator=(const std::initializer_list<T> &other)
        {
            return operator=<const std::initializer_list<T>>(other);  // NOLINT(misc-unconventional-assign-operator)
        }

        IndexAccessProxy operator[](const std::string &key);

    protected:
        const std::function<void(OpaqueItemPtr value_ptr)> assignment_callback_;
        OpaqueMapping *mapping_parent_;
    };

public:
    void build(OpaqueItemBuilder builder) override;

    IndexAccessProxy operator[](const std::string &key);

    void assign_pair(const std::string &key, OpaqueItemPtr value);

    const OpaqueItemPtr &get(const std::string &key);

private:
    std::unordered_map<std::string, OpaqueItemPtr> mapping_pair_;
};

}  // namespace msgx
