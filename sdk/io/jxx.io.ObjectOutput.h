#pragma once
#include "io/jxx.io.AutoCloseable.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.Flushable.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class Object; }
namespace jxx::io { class ObjectOutput : public ::jxx::lang::InterfaceBase<ObjectOutput,DataOutput,AutoCloseable,Flushable> { public: ~ObjectOutput() override=default; virtual void writeObject(const ::jxx::Ptr<::jxx::lang::Object>& object)=0; }; }
