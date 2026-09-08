#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::org::w3c::dom {
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
class Attr : public jxx::lang::InterfaceBase<Attr,Node> { public: ~Attr() override=default; virtual jxx::Ptr<jxx::lang::String> getName() const=0; virtual jxx::Ptr<jxx::lang::String> getValue() const=0; virtual void setValue(const jxx::Ptr<jxx::lang::String>& value)=0; };
} // namespace jxx::org::w3c::dom
