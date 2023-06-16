#include "bindable_item.h"
#include "mappings.h"

namespace msgx
{
class OpaqueMappingOrBindableItem
{
public:
    constexpr OpaqueMappingOrBindableItem() : uninitialised_dummy_{false}
    {
    }

    ~OpaqueMappingOrBindableItem()
    {
        destroy();
    }

    void destroy()
    {
        switch (type)
        {
            case unionType::Empty:
                break;
            case unionType::BindableItem:
                bindable_item_.~BindableOpaqueItem();
                break;
            case unionType::Mapping:
                mapping_.~OpaqueMapping();
                break;
        }
        type = unionType::Empty;
    }

    void set_mapping(msgx::OrphanageGetter get_orphanage_functor)
    {
        destroy();
        new (&mapping_) msgx::OpaqueMapping{std::move(get_orphanage_functor)};
        type = unionType::Mapping;
    }

    void set_bindable_item(msgx::OrphanageGetter get_orphanage_functor)
    {
        destroy();
        new (&bindable_item_) msgx::BindableOpaqueItem{std::move(get_orphanage_functor)};
        type = unionType::BindableItem;
    }

    enum class unionType
    {
        Empty,
        BindableItem,
        Mapping
    } type = unionType::Empty;  // match the default constructor of enum

    union
    {
        bool uninitialised_dummy_;
        msgx::OpaqueMapping mapping_;
        msgx::BindableOpaqueItem bindable_item_;
    };
};
}  // namespace msgx