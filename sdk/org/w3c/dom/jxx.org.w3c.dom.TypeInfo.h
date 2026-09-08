#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::org::w3c::dom { class TypeInfo : public jxx::lang::InterfaceBase<TypeInfo> { public: static constexpr jxx::lang::jint DERIVATION_RESTRICTION=1,DERIVATION_EXTENSION=2,DERIVATION_UNION=4,DERIVATION_LIST=8; ~TypeInfo() override=default; virtual jxx::Ptr<jxx::lang::String> getTypeName() const=0; virtual jxx::Ptr<jxx::lang::String> getTypeNamespace() const=0; virtual jxx::lang::jbool isDerivedFrom(const jxx::Ptr<jxx::lang::String>& ns,const jxx::Ptr<jxx::lang::String>& name,jxx::lang::jint method) const=0; }; }
