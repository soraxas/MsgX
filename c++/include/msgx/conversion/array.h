
#pragma once

#include "msgx/conversion/common.h"
#include "msgx/opaque_item/item.h"
#include "msgx/opaque_item/ndarray_dispatcher.h"

namespace msgx
{

namespace detail
{
template <typename DataType>
void numeric_ptr_to_ndarray(msgx::type::ndarray::NdArray::Builder ndarray_builder, const DataType *ptr,
                            size_t num_elements)
{
    // set data type
    ndarray_builder.setDtype(msgx::detail::DTypeToEnum<DataType>());
    // cast given pointer to byte, then calculate word size
    auto byte_ptr = reinterpret_cast<const byte *>(ptr);
    auto word_size = sizeof(DataType) * num_elements;
    ndarray_builder.setBuffer(kj::arrayPtr(byte_ptr, word_size));

    // assign device specific endianness, which is actually const
    ndarray_builder.setEndianness(msgx::helpers::get_device_specific_endian());
}
}  // namespace detail

namespace conversion
{

template <typename ContainerDataType, typename DataType = typename ContainerDataType::value_type,
          typename helpers::enable_if_is_numeric_t<DataType> * = nullptr>
void opaque_item(msgx::BindableOpaqueItem &item, const ContainerDataType &values)
{
    SPDLOG_DEBUG("[Conversion] stl container type {} with size '{}'", typeid(DataType).name(), values.size());

    auto _main_body = [](OpaqueItemBuilder builder, const ContainerDataType &values)
    { detail::numeric_ptr_to_ndarray<DataType>(builder.initNdArray(), values.data(), values.size()); };

    ::msgx::helpers::_run_callback_with_or_without_orphanage<ContainerDataType>(item, values, _main_body);
}

template <typename DataType, typename helpers::enable_if_is_numeric_t<DataType> * = nullptr>
void opaque_item(msgx::BindableOpaqueItem &item, std::initializer_list<DataType> values)
{
    SPDLOG_DEBUG("[Conversion] initializer_list type {} with size '{}'", typeid(DataType).name(), values.size());
    // move to a vector so that we can use the ptr

    std::vector<DataType> vector_values = std::move(values);
    msgx::conversion::opaque_item<std::vector<DataType>>(item, vector_values);
}

}  // namespace conversion
}  // namespace msgx
