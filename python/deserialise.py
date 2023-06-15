import numpy as np

import capnp
from msgx_capnp.msgx_capnp import Item

from sys import byteorder as BYTE_ORDER
from constants import ENDIANNESS_MAPPING, DATATYPE_MAPPING
from collections import namedtuple


NamedBinary = namedtuple("NamedBinary", "description bytes")

DECODE_MAPPING = {
    # primitives
    Item.Oneof.null.value: lambda x: None,
    Item.Oneof.bool.value: lambda x: x.bool,
    Item.Oneof.uChar.value: lambda x: x.uChar,
    Item.Oneof.uInt.value: lambda x: x.uInt,
    Item.Oneof.uLong.value: lambda x: x.uLong,
    Item.Oneof.uShort.value: lambda x: x.uShort,
    Item.Oneof.char.value: lambda x: x.char,
    Item.Oneof.short.value: lambda x: x.short,
    Item.Oneof.int.value: lambda x: x.int,
    Item.Oneof.long.value: lambda x: x.long,
    Item.Oneof.float.value: lambda x: x.float,
    Item.Oneof.double.value: lambda x: x.double,
    # basic pointer type
    Item.Oneof.string.value: lambda x: x.string,
    Item.Oneof.binary.value: lambda x: x.binary,
    Item.Oneof.namedBinary.value: lambda x: NamedBinary(
        x.namedBinary.description, x.namedBinary.bytes
    ),
    # list of pointers (trivial)
    Item.Oneof.boolList.value: lambda x: x.boolList,
    Item.Oneof.intList.value: lambda x: x.intList,
    Item.Oneof.longList.value: lambda x: x.longList,
    Item.Oneof.floatList.value: lambda x: x.floatList,
    Item.Oneof.doubleList.value: lambda x: x.doubleList,
    Item.Oneof.stringList.value: lambda x: x.stringList,
    # list of pointers
    Item.Oneof.anyList.value: lambda x: decode_list(x.anyList),
    Item.Oneof.mapping.value: lambda x: decode_mapping(x.mapping),
    # ndarray with binary buffer
    Item.Oneof.ndArray.value: lambda x: decode_ndarray(x.ndArray),
}


def decode_not_implemented(item_oneof):
    raise NotImplementedError(f"Item is not implemented: {item_oneof}")


def decode_mapping(value):
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
    return [decode_item(val) for val in value]


def decode_ndarray(value):
    if ENDIANNESS_MAPPING[value.endianness.raw] != BYTE_ORDER:
        raise RuntimeError(
            f"Endinness are different! Received an ndarray message with "
            f"endianness of {value.endianness}, but this system has {BYTE_ORDER} endian."
        )

    array = np.frombuffer(value.buffer, dtype=DATATYPE_MAPPING[value.dtype.raw])
    if value.which == "multiDimensional":
        raise NotImplementedError("nope")

    return array


def decode_item(opaque_item):
    _oneof = opaque_item.oneof
    return DECODE_MAPPING[_oneof.which.raw](_oneof)


def decode_message(byte_string):
    with Item.from_bytes(byte_string) as opaque_item:
        return decode_item(opaque_item)
