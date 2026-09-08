#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::org::w3c::dom {
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
class Text : public jxx::lang::InterfaceBase<Text,Node> { public: ~Text() override=default; virtual jxx::Ptr<jxx::lang::String> getData() const=0; virtual void setData(const jxx::Ptr<jxx::lang::String>& data)=0; };
} // namespace jxx::org::w3c::dom
