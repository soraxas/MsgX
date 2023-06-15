import numpy as np
import capnp
from msgx_capnp.ndarray_capnp import Endianness, DataType

ENDIANNESS_MAPPING = {
    Endianness.littleEndianOrder: "little",
    Endianness.bigEndianOrder: "big",
    Endianness.middleEndianOrder: None,
}

DATATYPE_MAPPING = {
    DataType.bool8: np.bool8,  # bool
    # signed
    DataType.int8: np.int8,  # char
    DataType.int16: np.int16,  # short
    DataType.int32: np.int32,  # int
    DataType.int64: np.int64,  # long
    # DataType.int128: np.int128,  # longLong
    # unsigned
    DataType.uInt8: np.uint8,  # uChar
    DataType.uInt16: np.uint16,  # uShort
    DataType.uInt32: np.uint32,  # uInt
    DataType.uInt64: np.uint64,  # uLong
    # DataType.uInt128: np.uint128,  # uLongLong
    # floating point
    DataType.float16: np.float16,  # half
    DataType.float32: np.float32,  # single
    DataType.float64: np.float64,  # double
    DataType.float128: np.float128,  # longDouble
    # complex floating point
    # DataType.cFloat32: np.complex32,  # cHalf
    DataType.cFloat64: np.complex64,  # cSingle
    DataType.cFloat128: np.complex128,  # cDouble
    DataType.cFloat256: np.complex256,  # cLongDouble
}
