#pragma once

#if defined _GLIBCXX_COMPLEX || defined MSGX_USE_COMPLEX
#define MSGX_USE_COMPLEX
#include <complex>
#endif

#include <math.h>

#include <exception>

#include "msgx.capnp.h"

namespace msgx
{
namespace detail
{
template <typename DataType>
constexpr auto DTypeToEnum()
{
    std::terminate();
}

template <>
constexpr auto DTypeToEnum<bool>()
{
    return msgx::type::ndarray::DataType::BOOL8;
}

template <>
constexpr auto DTypeToEnum<int8_t>()
{
    return msgx::type::ndarray::DataType::INT8;
}

template <>
constexpr auto DTypeToEnum<int16_t>()
{
    return msgx::type::ndarray::DataType::INT16;
}

template <>
constexpr auto DTypeToEnum<int32_t>()
{
    return msgx::type::ndarray::DataType::INT32;
}

template <>
constexpr auto DTypeToEnum<int64_t>()
{
    return msgx::type::ndarray::DataType::INT64;
}

// template <>
// constexpr auto DTypeToEnum<int128_t>()
//{
//     return msgx::type::ndarray::DataType::INT128;
// }

template <>
constexpr auto DTypeToEnum<uint8_t>()
{
    return msgx::type::ndarray::DataType::U_INT8;
}

template <>
constexpr auto DTypeToEnum<uint16_t>()
{
    return msgx::type::ndarray::DataType::U_INT16;
}

template <>
constexpr auto DTypeToEnum<uint32_t>()
{
    return msgx::type::ndarray::DataType::U_INT32;
}

template <>
constexpr auto DTypeToEnum<uint64_t>()
{
    return msgx::type::ndarray::DataType::U_INT64;
}

// template <>
// constexpr auto DTypeToEnum<uint128_t>()
//{
//     return msgx::type::ndarray::DataType::U_INT128;
// }

// template <>
// constexpr auto DTypeToEnum<>()
//{
//     return msgx::type::ndarray::DataType::FLOAT16;
// }

template <>
constexpr auto DTypeToEnum<float_t>()
{
    return msgx::type::ndarray::DataType::FLOAT32;
}

template <>
constexpr auto DTypeToEnum<double_t>()
{
    return msgx::type::ndarray::DataType::FLOAT64;
}

// template <>
// constexpr auto DTypeToEnum<>()
//{
//     return msgx::type::ndarray::DataType::FLOAT128;
// }

// template <>
// constexpr auto DTypeToEnum<>()
//{
//     return msgx::type::ndarray::DataType::C_FLOAT32;
// }

#ifdef MSGX_USE_COMPLEX
template <>
constexpr auto DTypeToEnum<std::complex<float_t>>()
{
    return msgx::type::ndarray::DataType::C_FLOAT64;
}

template <>
constexpr auto DTypeToEnum<std::complex<double_t>>()
{
    return msgx::type::ndarray::DataType::C_FLOAT128;
}
#endif

// template <>
// constexpr auto DTypeToEnum<>()
//{
//     return msgx::type::ndarray::DataType::C_FLOAT256;
// }
}  // namespace detail
}  // namespace msgx