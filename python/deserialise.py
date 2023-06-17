import numpy as np

from sys import byteorder
from collections import namedtuple

from .msg_definitions import msgx_capnp
from .constants import ENDIANNESS_MAPPING, DATATYPE_MAPPING

NamedBinary = namedtuple("NamedBinary", "description bytes")

Oneof = msgx_capnp.Item.Oneof

DECODE_MAPPING = {
    # primitives
    Oneof.null.value: lambda x: None,
    Oneof.bool.value: lambda x: x.bool,
    Oneof.uChar.value: lambda x: x.uChar,
    Oneof.uInt.value: lambda x: x.uInt,
    Oneof.uLong.value: lambda x: x.uLong,
    Oneof.uShort.value: lambda x: x.uShort,
    Oneof.char.value: lambda x: x.char,
    Oneof.short.value: lambda x: x.short,
    Oneof.int.value: lambda x: x.int,
    Oneof.long.value: lambda x: x.long,
    Oneof.float.value: lambda x: x.float,
    Oneof.double.value: lambda x: x.double,
    # basic pointer type
    Oneof.string.value: lambda x: x.string,
    Oneof.binary.value: lambda x: x.binary,
    Oneof.namedBinary.value: lambda x: NamedBinary(
        x.namedBinary.description, x.namedBinary.bytes
    ),
    # list of pointers (trivial)
    Oneof.boolList.value: lambda x: x.boolList,
    Oneof.intList.value: lambda x: x.intList,
    Oneof.longList.value: lambda x: x.longList,
    Oneof.floatList.value: lambda x: x.floatList,
    Oneof.doubleList.value: lambda x: x.doubleList,
    Oneof.stringList.value: lambda x: x.stringList,
    # list of pointers
    Oneof.anyList.value: lambda x: decode_list(x.anyList),
    Oneof.mapping.value: lambda x: decode_mapping(x.mapping),
    # ndarray with binary buffer
    Oneof.ndArray.value: lambda x: decode_ndarray(x.ndArray),
}


def decode_not_implemented(item_oneof):
    """Default decode function for things that are not implemented."""
    raise NotImplementedError(f"Item is not implemented: {item_oneof}")


def decode_mapping(value):
    """Decode function for nest mapping type."""
    _dict = dict()
    for entry in value.entries:
        if entry.key in _dict:
            print(f"==== WARN ====")
            print(f"> The key {entry.key} is repeated, it already exists!")
            print(f"> Going to use the first instance and ignore the repeated key")
            continue
        _dict[entry.key] = decode_item(entry.value)
    return _dict


def decode_list(value):
    """Decode function for nested list type."""
    return [decode_item(val) for val in value]


def decode_ndarray(value):
    """Decode function for packed numeric type."""
    if ENDIANNESS_MAPPING[value.endianness.raw] != byteorder:
        raise RuntimeError(
            f"Endinness are different! Received an ndarray message with "
            f"endianness of {value.endianness}, but this machin has {byteorder} endian."
        )

    array = np.frombuffer(value.buffer, dtype=DATATYPE_MAPPING[value.dtype.raw])
    if value.which == "multiDimensional":
        raise NotImplementedError("nope")

    return array


def decode_item(opaque_item):
    """Decode function for opaque item type."""
    _oneof = opaque_item.oneof
    return DECODE_MAPPING[_oneof.which.raw](_oneof)


def decode_message(byte_string):
    """The entry function to transform a byte string into python native objects."""
    with msgx_capnp.Item.from_bytes(byte_string) as opaque_item:
        return decode_item(opaque_item)
