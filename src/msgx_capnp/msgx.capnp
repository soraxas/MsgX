@0xae43b38254dba6ca;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("msgx::type");

using NdArray = import "ndarray.capnp";

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

        boolList           @15 :List(Bool);
        intList            @16 :List(Int32);
        longList           @17 :List(Int64);
        floatList          @18 :List(Float32);
        doubleList         @19 :List(Float64);
        stringList         @20 :List(Text);
        anyList            @21 :List(Item);

        ndArray            @22 :NdArray.NdArray;

        mapping            @23 :Map(Text, Item);
    }
}

