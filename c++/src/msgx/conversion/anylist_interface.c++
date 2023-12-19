#include "msgx/conversion/anylist_interface.h"

namespace msgx
{

OrphanOpaqueItem AnyList(OrphanageGetter orphanage_getter, OpaqueItemList &container) {
    auto item_orphan = orphanage_getter().newOrphan<msgx::type::Item::Oneof>();
    auto anylist_builder = item_orphan.get().initAnyList(container.size());
//
    for (size_t i = 0; i < container.size(); ++i)
{
    auto orphan = orphanage_getter().newOrphan<msgx::type::Item>();

    container[i]->build(orphan.get().initOneof());
    anylist_builder.adoptWithCaveats(i, std::move(orphan));
}

return item_orphan;
}

}  // namespace msgx