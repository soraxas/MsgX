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

template <typename T, typename std::enable_if<std::is_same<T, float_t>::value, int>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initFloatArray;
}

template <typename T, typename std::enable_if<std::is_same<T, double_t>::value, int>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initDoubleArray;
}

template <typename T, typename std::enable_if<std::is_same<T, uint8_t>::value ||   //
                                              std::is_same<T, uint16_t>::value ||  //
                                              std::is_same<T, int8_t>::value ||    //
                                              std::is_same<T, int16_t>::value ||   //
                                              std::is_same<T, int32_t>::value>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initIntArray;
}

template <typename T, typename std::enable_if<std::is_same<T, uint32_t>::value ||  //
                                              std::is_same<T, uint64_t>::value ||  // lossy
                                              std::is_same<T, int64_t>::value>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initLongArray;
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, int>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initBoolArray;
}

template <typename T, typename std::enable_if<std::is_same<T, const char *>::value ||  //
                                              std::is_same<T, std::string>::value>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initStringArray;
}

template <typename T, typename std::enable_if<std::is_same<T, msgx::type::Item>::value, int>::type * = nullptr>
constexpr auto NdArrayInitArrayDispatcher()
{
    return &msgx::type::Item::Oneof::Builder::initAnyArray;
}

////////////////////////////////////////////////////////////

}  // namespace conversion
}  // namespace msgx