#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

namespace jxx::org::w3c::dom {

class Attr;
class NodeList;
class TypeInfo;

class Element : public ::jxx::lang::InterfaceBase<Element, Node> {
public:
    ~Element() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getTagName() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual void setAttribute(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<::jxx::lang::String>& value) = 0;
    virtual void removeAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) = 0;
    virtual ::jxx::Ptr<Attr> getAttributeNode(const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual ::jxx::Ptr<Attr> setAttributeNode(const ::jxx::Ptr<Attr>& attribute) = 0;
    virtual ::jxx::Ptr<Attr> removeAttributeNode(const ::jxx::Ptr<Attr>& attribute) = 0;
    virtual ::jxx::Ptr<NodeList> getElementsByTagName(const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getAttributeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;
    virtual void setAttributeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& qualifiedName, const ::jxx::Ptr<::jxx::lang::String>& value) = 0;
    virtual void removeAttributeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName) = 0;
    virtual ::jxx::Ptr<Attr> getAttributeNodeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;
    virtual ::jxx::Ptr<Attr> setAttributeNodeNS(const ::jxx::Ptr<Attr>& attribute) = 0;
    virtual ::jxx::Ptr<NodeList> getElementsByTagNameNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;
    virtual ::jxx::lang::jbool hasAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual ::jxx::lang::jbool hasAttributeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;
    virtual ::jxx::Ptr<TypeInfo> getSchemaTypeInfo() const = 0;
    virtual void setIdAttribute(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jbool isId) = 0;
    virtual void setIdAttributeNS(const ::jxx::Ptr<::jxx::lang::String>& namespaceURI, const ::jxx::Ptr<::jxx::lang::String>& localName, ::jxx::lang::jbool isId) = 0;
    virtual void setIdAttributeNode(const ::jxx::Ptr<Attr>& attribute, ::jxx::lang::jbool isId) = 0;
};

} // namespace jxx::org::w3c::dom
