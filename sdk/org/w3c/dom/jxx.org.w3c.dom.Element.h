#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::org::w3c::dom {
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
class NodeList;
class Element : public jxx::lang::InterfaceBase<Element,Node> { public: ~Element() override=default; virtual jxx::Ptr<jxx::lang::String> getTagName() const=0; virtual jxx::Ptr<jxx::lang::String> getAttribute(const jxx::Ptr<jxx::lang::String>& name) const=0; virtual void setAttribute(const jxx::Ptr<jxx::lang::String>& name,const jxx::Ptr<jxx::lang::String>& value)=0; virtual void removeAttribute(const jxx::Ptr<jxx::lang::String>& name)=0; virtual jxx::lang::jbool hasAttribute(const jxx::Ptr<jxx::lang::String>& name) const=0; virtual jxx::Ptr<NodeList> getElementsByTagName(const jxx::Ptr<jxx::lang::String>& name) const=0; };
} // namespace jxx::org::w3c::dom
