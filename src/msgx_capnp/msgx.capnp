@0xae43b38254dba6ca;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("msgx::type");

#using Math = import "math.capnp";

struct NdArray(DataType) {
    buffer                 @0: DataType;

    union {
        oneDimensional     @1: Void;
        multiDimensional :group {
            columnMajor    @2: Bool;
            shape          @3: List(UInt32);
        }
    }
}

struct NamedBinary {
    description            @0 :Text;
    bytes                  @1 :Data;
}

struct Map(Key, Value) {
    entries                @0 :List(Entry);
    struct Entry {
        key                @0 :Key;
        value              @1 :Value;
    }
}

struct Item {
    oneof :union {
        null               @0 :Void;

        bool               @1 :Bool;

        char               @2 :Int8;
        short              @3 :Int16;
        int                @4 :Int32;
        long               @5 :Int64;

        uChar              @6 :UInt8;
        uShort             @7 :UInt16;
        uInt               @8 :UInt32;
        uLong              @9 :UInt64;

        float              @10 :Float32;
        double             @11 :Float64;

        string             @12 :Text;
        binary             @13 :Data;
        namedBinary        @14 :NamedBinary;

        boolArray          @15 :NdArray(List(Bool));
        intArray           @16 :NdArray(List(Int32));
        longArray          @17 :NdArray(List(Int64));
        floatArray         @18 :NdArray(List(Float32));
        doubleArray        @19 :NdArray(List(Float64));
        stringArray        @20 :NdArray(List(Text));
        anyArray           @21 :NdArray(List(Item));

        mapping            @22 :Map(Text, Item);
    }
}

