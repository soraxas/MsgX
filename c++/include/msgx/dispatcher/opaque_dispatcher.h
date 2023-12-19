#pragma once

#include "msgx.capnp.h"

namespace msgx
{
namespace detail
{
template <typename DataType>
constexpr auto NdListAdoptOrphanDispatcher()
{
    std::terminate();
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<double_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptDoubleList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<float_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptFloatList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<int32_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptIntList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<int64_t>()
{
    return &msgx::type::Item::Oneof::Builder::adoptLongList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<bool>()
{
    return &msgx::type::Item::Oneof::Builder::adoptBoolList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<std::string>()
{
    return &msgx::type::Item::Oneof::Builder::adoptStringList;
}

template <>
constexpr auto NdListAdoptOrphanDispatcher<msgx::type::Item>()
{
    return &msgx::type::Item::Oneof::Builder::adoptAnyList;
}

////////////////////////////////////////////////////////////

template <typename T, typename std::enable_if<std::is_same<T, float_t>::value, int>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initFloatList;
}

template <typename T, typename std::enable_if<std::is_same<T, double_t>::value, int>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initDoubleList;
}

template <typename T, typename std::enable_if<std::is_same<T, uint8_t>::value ||   //
                                              std::is_same<T, uint16_t>::value ||  //
                                              std::is_same<T, int8_t>::value ||    //
                                              std::is_same<T, int16_t>::value ||   //
                                              std::is_same<T, int32_t>::value>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initIntList;
}

template <typename T, typename std::enable_if<std::is_same<T, uint32_t>::value ||  //
                                              std::is_same<T, uint64_t>::value ||  // lossy
                                              std::is_same<T, int64_t>::value>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initLongList;
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, int>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initBoolList;
}

template <typename T, typename std::enable_if<std::is_same<T, const char *>::value ||  //
                                              std::is_same<T, std::string>::value>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initStringList;
}

template <typename T, typename std::enable_if<std::is_same<T, msgx::type::Item>::value, int>::type * = nullptr>
constexpr auto InitListDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initAnyList;
}

////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace msgx