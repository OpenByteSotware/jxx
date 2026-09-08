#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::org::w3c::dom {
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
class Element; class Text; class Attr; class NodeList; class DOMImplementation;
class Document : public jxx::lang::InterfaceBase<Document,Node> { public: ~Document() override=default; virtual jxx::Ptr<Element> getDocumentElement() const=0; virtual jxx::Ptr<Element> createElement(const jxx::Ptr<jxx::lang::String>& tagName)=0; virtual jxx::Ptr<Text> createTextNode(const jxx::Ptr<jxx::lang::String>& data)=0; virtual jxx::Ptr<Attr> createAttribute(const jxx::Ptr<jxx::lang::String>& name)=0; virtual jxx::Ptr<NodeList> getElementsByTagName(const jxx::Ptr<jxx::lang::String>& tagName) const=0; virtual jxx::Ptr<DOMImplementation> getImplementation() const=0; };
} // namespace jxx::org::w3c::dom
