#ifndef MSGX_CAPNP_CONVERSION_H
#define MSGX_CAPNP_CONVERSION_H

#include <soraxas_toolbox/format.h>

#include <sstream>
#include <vector>

namespace msgx
{
namespace helpers
{
template <typename T, typename ListReader>
std::vector<T> get_capnp_list(ListReader reader)
{
    static_assert(sizeof(T) == sizeof(decltype(*reader.begin())), "size mismatch");
    std::vector<T> container(reader.size());
    std::copy(reader.begin(), reader.end(), container.begin());
    return container;
}

template <typename T, typename ListBuilder>
ListBuilder build_capnp_list(ListBuilder builder, const std::vector<T> &values)
// auto build_capnp_list(typename capnp::List<T>::Builder builder, const std::vector<U> &values)
{
    if (builder.size() != values.size())
        throw std::runtime_error(sxs::StringStreamer() << "CapnpBuilder has size " << builder.size()
                                                       << ", but input list has size " << values.size());

    for (size_t i = 0; i < values.size(); ++i)
        builder.set(i, values[i]);
    return builder;
}
}  // namespace helpers
}  // namespace msgx

#endif  // MSGX_CAPNP_CONVERSION_H
