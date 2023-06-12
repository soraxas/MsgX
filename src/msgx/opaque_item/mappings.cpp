

#include "msgx/opaque_item/mappings.h"

#include <stdexcept>
#include <utility>

namespace msgx
{

OpaqueMapping::IndexAccessProxy::IndexAccessProxy(
    const std::function<void(const OpaqueItemPtr value_ptr)> &assignment_callback, OpaqueMapping *mapping_parent)
  : assignment_callback_(assignment_callback), mapping_parent_(mapping_parent)
{
}

void OpaqueMapping::build(msgx::type::Item::Oneof::Builder builder)
{
    auto mapping_builder = builder.initMapping();
    auto entry_builder = mapping_builder.initEntries(mapping_pair.size());

    size_t i = 0;
    for (auto &&keyval : mapping_pair)
    {
        if (!keyval.second)
            throw std::runtime_error("Value is null for item with key" + keyval.first);
        entry_builder[i].setKey(keyval.first);
        keyval.second->build(entry_builder[i].initValue().initOneof());
        ++i;
    }
}

OpaqueMapping::IndexAccessProxy OpaqueMapping::operator[](const std::string &key)
{
    auto callback = [this, key](const OpaqueItemPtr &value_ptr) { assign_pair(key, value_ptr); };

    return OpaqueMapping::IndexAccessProxy(callback, this);
}

OpaqueMapping::IndexAccessProxy OpaqueMapping::IndexAccessProxy::operator[](const std::string &key)
{
    // my value would now be a mapping as well,
    // as a nested structure

    auto nested_callback = [this, key](const OpaqueItemPtr &value_ptr)
    {
        // only call the actual callback if some concrete value is assigned
        // as the result of this index

        // create a new instance of a nested mapping
        auto nested_mapping = std::make_shared<OpaqueMapping>(mapping_parent_->get_orphanage_functor_);

        // actually assign the pair (instead of using [], which will triggers index proxy again)
        nested_mapping->assign_pair(key, value_ptr);

        // callback our previous caller
        assignment_callback_(nested_mapping);

        // now, changes will bubble up
    };
    return IndexAccessProxy(nested_callback, mapping_parent_);
}

void OpaqueMapping::assign_pair(const std::string &key, OpaqueItemPtr value)
{
    mapping_pair[key] = std::move(value);
}

}  // namespace msgx