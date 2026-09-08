#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::org::w3c::dom {
class Node;
class NamedNodeMap : public jxx::lang::InterfaceBase<NamedNodeMap> { public: ~NamedNodeMap() override=default; virtual jxx::Ptr<Node> getNamedItem(const jxx::Ptr<jxx::lang::String>& name) const=0; virtual jxx::Ptr<Node> setNamedItem(const jxx::Ptr<Node>& node)=0; virtual jxx::Ptr<Node> removeNamedItem(const jxx::Ptr<jxx::lang::String>& name)=0; virtual jxx::Ptr<Node> item(jxx::lang::jint index) const=0; virtual jxx::lang::jint getLength() const=0; };
} // namespace jxx::org::w3c::dom
