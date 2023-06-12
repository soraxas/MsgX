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
    return &msgx::type::Item::Oneof::Builder::adoptDoubleArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<float_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptFloatArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<int32_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptIntArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<int64_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptLongArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<bool>()
{
    return &msgx::type::Item::Oneof::Builder::adoptBoolArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<std::string>()
{
    return &msgx::type::Item::Oneof::Builder::adoptStringArray;
};

template <>
constexpr auto NdArrayAdoptOrphanDispatcher<msgx::type::Item>()
{
    return &msgx::type::Item::Oneof::Builder::adoptAnyArray;
};

////////////////////////////////////////////////////////////

template <typename DataType>
constexpr auto NdArrayInitArrayDispatcher()
{
    std::terminate();
}

template <>
constexpr auto NdArrayInitArrayDispatcher<double_t>()
{
    return &msgx::type::Item::Oneof::Builder::initDoubleArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<float_t>()
{
    return &msgx::type::Item::Oneof::Builder::initFloatArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<int32_t>()
{
    return &msgx::type::Item::Oneof::Builder::initIntArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<int64_t>()
{
    return &msgx::type::Item::Oneof::Builder::initLongArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<bool>()
{
    return &msgx::type::Item::Oneof::Builder::initBoolArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<std::string>()
{
    return &msgx::type::Item::Oneof::Builder::initStringArray;
};

template <>
constexpr auto NdArrayInitArrayDispatcher<msgx::type::Item>()
{
    return &msgx::type::Item::Oneof::Builder::initAnyArray;
};

////////////////////////////////////////////////////////////

}  // namespace conversion
}  // namespace msgx