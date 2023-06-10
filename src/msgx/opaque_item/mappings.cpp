

#include "msgx/opaque_item/mappings.h"

#include <array>
#include <stdexcept>
#include <utility>

namespace msgx
{

OpaqueMapping::IndexAccessProxy::IndexAccessProxy(
    const std::function<void(const OpaqueItemPtr value_ptr)> &assignment_callback, OpaqueMapping *mapping_parent)
  : assignment_callback_(assignment_callback), mapping_parent_(mapping_parent)
{
}

void OpaqueMapping::build(msgx::Item::Oneof::Builder builder)
{
    auto mapping_builder = builder.initMapping();
    auto entry_builder = mapping_builder.initEntries(mapping_pair.size());
    for (auto i = 0; i < mapping_pair.size(); ++i)
    {
        if (!mapping_pair[i][0])
            throw std::runtime_error("Key is null");
        if (!mapping_pair[i][1])
            throw std::runtime_error("Value is null");

        mapping_pair[i][0]->build(entry_builder[i].initKey().initOneof());
        mapping_pair[i][1]->build(entry_builder[i].initValue().initOneof());
    }
}

void OpaqueMapping::assign_pair(OpaqueItemPtr key, OpaqueItemPtr value)
{
    mapping_pair.emplace_back();
    size_t idx = mapping_pair.size() - 1;
    mapping_pair[idx][0] = std::move(key);
    mapping_pair[idx][1] = std::move(value);
}

}  // namespace msgx