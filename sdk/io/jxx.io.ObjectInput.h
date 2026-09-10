#pragma once
#include "io/jxx.io.AutoCloseable.h"
#include "io/jxx.io.DataInput.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class Object; }
namespace jxx::io { class ObjectInput : public ::jxx::lang::InterfaceBase<ObjectInput,DataInput,AutoCloseable> { public: ~ObjectInput() override=default; virtual ::jxx::Ptr<::jxx::lang::Object> readObject()=0; virtual ::jxx::lang::jint read()=0; virtual ::jxx::lang::jint read(const ::jxx::lang::ByteArray& b)=0; virtual ::jxx::lang::jint read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint off,::jxx::lang::jint len)=0; virtual ::jxx::lang::jlong skip(::jxx::lang::jlong n)=0; virtual ::jxx::lang::jint available()=0; }; }
