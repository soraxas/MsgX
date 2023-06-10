#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace msgx
{
class OpaqueItem
{
public:
    explicit OpaqueItem()
    {
    }

    virtual void build(msgx::Item::Oneof::Builder builder) = 0;
};

template <typename CapnpItemType>
class OpaqueComposedItem : public OpaqueItem
{
public:
    explicit OpaqueComposedItem(const std::function<capnp::Orphanage(void)> &get_orphange_functor)
      : get_orphange_functor_(get_orphange_functor)
    {
    }

protected:
    capnp::BuilderFor<CapnpItemType> createOrphanGetBuilder()
    {
        orphan_ = get_orphange_functor_().newOrphan<CapnpItemType>();
        return orphan_.get();
    }

protected:
    const std::function<capnp::Orphanage(void)> &get_orphange_functor_;

    capnp::Orphan<CapnpItemType> orphan_;
};

using OpaqueItemPtr = std::shared_ptr<OpaqueItem>;

}  // namespace msgx