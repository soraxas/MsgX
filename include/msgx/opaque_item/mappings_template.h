#pragma once

namespace msgx
{

template <typename T>
OpaqueMapping::IndexAccessProxy OpaqueMapping::operator[](const T &key)
{
    auto callback = [this, key](const OpaqueItemPtr &value_ptr)
    { assign_pair(::msgx::conversion::opaque_item(get_orphange_functor_, key), value_ptr); };

    return OpaqueMapping::IndexAccessProxy(callback, this);
}

template <typename T>
OpaqueMapping::IndexAccessProxy &OpaqueMapping::IndexAccessProxy::operator=(T &other)
{
    assignment_callback_(
        ::msgx::conversion::opaque_item(mapping_parent_->get_orphange_functor_, std::forward<T>(other)));
    return *this;
}

// r-value
template <typename T>
OpaqueMapping::IndexAccessProxy &OpaqueMapping::IndexAccessProxy::operator=(T &&other)
{
    operator=(other);
    return *this;
}

template <typename T>
OpaqueMapping::IndexAccessProxy OpaqueMapping::IndexAccessProxy::operator[](const T &key)
{
    // my value would now be a mapping as well,
    // as a nested structure

    auto nested_callback = [this, key](const OpaqueItemPtr &value_ptr)
    {
        // only call the actual callback if some concrete value is assigned
        // as the result of this index

        // create a new instance of a nested mapping
        auto nested_mapping = std::make_shared<OpaqueMapping>(mapping_parent_->get_orphange_functor_);

        // actually assign the pair (instead of using [], which will triggers index proxy again)
        nested_mapping->assign_pair(::msgx::conversion::opaque_item(mapping_parent_->get_orphange_functor_, key),
                                    value_ptr);

        // callback our previous caller
        assignment_callback_(nested_mapping);

        // now, changes will bubble up
    };
    return IndexAccessProxy(nested_callback, mapping_parent_);
}
}  // namespace msgx