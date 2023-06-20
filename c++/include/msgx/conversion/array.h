#pragma once

#include "msgx/conversion/common.h"
#include "msgx/conversion/convertible.h"
#include "msgx/dispatcher/ndarray_dispatcher.h"
#include "msgx/opaque_item/item.h"

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
    const auto byte_ptr = reinterpret_cast<const byte *>(ptr);
    const auto word_size = sizeof(DataType) * num_elements;
    ndarray_builder.setBuffer(kj::arrayPtr(byte_ptr, word_size));
    ndarray_builder.setOneDimensional(num_elements);

    // assign device specific endianness, which is actually const
    ndarray_builder.setEndianness(msgx::helpers::get_device_specific_endian());
}
}  // namespace detail

template <typename Container>
struct conversion<Container,
                  typename std::enable_if<                                      //
                      helpers::is_numeric_v<typename Container::value_type> &&  // is numeric
                          helpers::has_data_ptr_v<Container> &&                 // has data ptr member
                          !helpers::is_eigen_matrix_type_v<Container>,          // not eigen
                      void>::type>
{
    static void convert(OpaqueItemBuilder builder, const Container &values)
    {
        SPDLOG_DEBUG("[Conversion] stl container type {} with size '{}'", typeid(Container::value_type).name(),
                     values.size());

        detail::numeric_ptr_to_ndarray<typename Container::value_type>(builder.initNdArray(), values.data(),
                                                                       values.size());
    }
};

template <typename DataType>
struct conversion<std::initializer_list<DataType>,
                  typename std::enable_if<helpers::is_numeric_v<DataType>,  // is numeric
                                          void>::type>
{
    static void convert(OpaqueItemBuilder builder, const std::initializer_list<DataType> &values)
    {
        std::vector<DataType> vector_values = std::move(values);
        msgx::conversion<std::vector<DataType>>::convert(builder, vector_values);
    }
};

}  // namespace msgx
