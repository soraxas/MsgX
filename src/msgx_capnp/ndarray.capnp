@0xdf12285c4a40f72b;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("msgx::type::ndarray");

enum Endianness {
    unspecified            @0;
    littleEndianOrder      @1;
    bigEndianOrder         @2;
    middleEndianOrder      @3;
}

enum DataType {
    bool8                  @0;   # bool
    # signed
    int8                   @1;   # char
    int16                  @2;   # short
    int32                  @3;   # int
    int64                  @4;   # long
    int128                 @5;   # longLong
    # unsigned
    uInt8                  @6;   # uChar
    uInt16                 @7;   # uShort
    uInt32                 @8;   # uInt
    uInt64                 @9;   # uLong
    uInt128                @10;  # uLongLong
    # floating point
    float16                @11;  # half
    float32                @12;  # single
    float64                @13;  # double
    float128               @14;  # longDouble
    # complex floating point
    cFloat32               @15;  # cHalf
    cFloat64               @16;  # cSingle
    cFloat128              @17;  # cDouble
    cFloat256              @18;  # cLongDouble
}

struct NdArray {
    buffer                 @0: Data;

    union {
        oneDimensional     @1: Void;
        multiDimensional :group {
            columnMajor    @2: Bool;
            shape          @3: List(UInt32);
        }
    }

    dtype                  @4: DataType;
    endianness             @5: Endianness;
}