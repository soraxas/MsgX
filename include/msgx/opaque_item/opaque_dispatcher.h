#pragma once

#include "msgx.capnp.h"

namespace msgx
{
namespace conversion
{
template <typename DataType>
constexpr auto NdArrayAdoptOrphanDispatcher()
{
    std::terminate();
}

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<double_t>()
{
    return &msgx::Item::Oneof::Builder::adoptDoubleArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<float_t>()
{
    return &msgx::Item::Oneof::Builder::adoptFloatArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<int32_t>()
{
    return &msgx::Item::Oneof::Builder::adoptIntArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<int64_t>()
{
    return &msgx::Item::Oneof::Builder::adoptLongArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<bool>()
{
    return &msgx::Item::Oneof::Builder::adoptBoolArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<std::string>()
{
    return &msgx::Item::Oneof::Builder::adoptStringArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<msgx::Item>()
{
    return &msgx::Item::Oneof::Builder::adoptAnyArray;
};


////////////////////////////////////////////////////////////



}  // namespace detail
}  // namespace msgx