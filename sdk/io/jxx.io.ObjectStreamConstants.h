#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io {
class ObjectStreamConstants : public ::jxx::lang::InterfaceBase<ObjectStreamConstants> {
public:
    static constexpr ::jxx::lang::jshort STREAM_MAGIC = static_cast<::jxx::lang::jshort>(0xACED);
    static constexpr ::jxx::lang::jshort STREAM_VERSION = 5;
    static constexpr ::jxx::lang::jbyte TC_BASE = static_cast<::jxx::lang::jbyte>(0x70);
    static constexpr ::jxx::lang::jbyte TC_NULL = static_cast<::jxx::lang::jbyte>(0x70);
    static constexpr ::jxx::lang::jbyte TC_REFERENCE = static_cast<::jxx::lang::jbyte>(0x71);
    static constexpr ::jxx::lang::jbyte TC_CLASSDESC = static_cast<::jxx::lang::jbyte>(0x72);
    static constexpr ::jxx::lang::jbyte TC_OBJECT = static_cast<::jxx::lang::jbyte>(0x73);
    static constexpr ::jxx::lang::jbyte TC_STRING = static_cast<::jxx::lang::jbyte>(0x74);
    static constexpr ::jxx::lang::jbyte TC_ARRAY = static_cast<::jxx::lang::jbyte>(0x75);
    static constexpr ::jxx::lang::jbyte TC_CLASS = static_cast<::jxx::lang::jbyte>(0x76);
    static constexpr ::jxx::lang::jbyte TC_BLOCKDATA = static_cast<::jxx::lang::jbyte>(0x77);
    static constexpr ::jxx::lang::jbyte TC_ENDBLOCKDATA = static_cast<::jxx::lang::jbyte>(0x78);
    static constexpr ::jxx::lang::jbyte TC_RESET = static_cast<::jxx::lang::jbyte>(0x79);
    static constexpr ::jxx::lang::jbyte TC_BLOCKDATALONG = static_cast<::jxx::lang::jbyte>(0x7A);
    static constexpr ::jxx::lang::jbyte TC_EXCEPTION = static_cast<::jxx::lang::jbyte>(0x7B);
    static constexpr ::jxx::lang::jbyte TC_LONGSTRING = static_cast<::jxx::lang::jbyte>(0x7C);
    static constexpr ::jxx::lang::jbyte TC_PROXYCLASSDESC = static_cast<::jxx::lang::jbyte>(0x7D);
    static constexpr ::jxx::lang::jbyte TC_ENUM = static_cast<::jxx::lang::jbyte>(0x7E);
    static constexpr ::jxx::lang::jbyte TC_MAX = static_cast<::jxx::lang::jbyte>(0x7E);
    static constexpr ::jxx::lang::jint baseWireHandle = 0x7E0000;
    static constexpr ::jxx::lang::jbyte SC_WRITE_METHOD = 0x01;
    static constexpr ::jxx::lang::jbyte SC_SERIALIZABLE = 0x02;
    static constexpr ::jxx::lang::jbyte SC_EXTERNALIZABLE = 0x04;
    static constexpr ::jxx::lang::jbyte SC_BLOCK_DATA = 0x08;
    static constexpr ::jxx::lang::jbyte SC_ENUM = 0x10;
    static constexpr ::jxx::lang::jint PROTOCOL_VERSION_1 = 1;
    static constexpr ::jxx::lang::jint PROTOCOL_VERSION_2 = 2;
protected:
    ~ObjectStreamConstants() override = default;
};
}
