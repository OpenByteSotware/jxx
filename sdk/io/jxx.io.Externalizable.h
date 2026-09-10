#pragma once
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io { class ObjectInput; class ObjectOutput; class Externalizable : public ::jxx::lang::InterfaceBase<Externalizable,Serializable> { public: ~Externalizable() override=default; virtual void writeExternal(const ::jxx::Ptr<ObjectOutput>& out)=0; virtual void readExternal(const ::jxx::Ptr<ObjectInput>& in)=0; }; }
