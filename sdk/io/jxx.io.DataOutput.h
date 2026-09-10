#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
namespace jxx::lang { class String; }
namespace jxx::io {
class DataOutput : public ::jxx::lang::InterfaceBase<DataOutput> { public: ~DataOutput() override=default; virtual void write(::jxx::lang::jint b)=0; virtual void write(const ::jxx::lang::ByteArray& b)=0; virtual void write(const ::jxx::lang::ByteArray& b,::jxx::lang::jint off,::jxx::lang::jint len)=0; virtual void writeBoolean(::jxx::lang::jbool v)=0; virtual void writeByte(::jxx::lang::jint v)=0; virtual void writeShort(::jxx::lang::jint v)=0; virtual void writeChar(::jxx::lang::jint v)=0; virtual void writeInt(::jxx::lang::jint v)=0; virtual void writeLong(::jxx::lang::jlong v)=0; virtual void writeFloat(::jxx::lang::jfloat v)=0; virtual void writeDouble(::jxx::lang::jdouble v)=0; virtual void writeBytes(const ::jxx::Ptr<::jxx::lang::String>& s)=0; virtual void writeChars(const ::jxx::Ptr<::jxx::lang::String>& s)=0; virtual void writeUTF(const ::jxx::Ptr<::jxx::lang::String>& s)=0; };
} // namespace jxx::io
