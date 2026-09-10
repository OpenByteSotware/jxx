#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

namespace jxx::org::w3c::dom {

class Attr;
class CDATASection;
class Comment;
class DOMImplementation;
class DocumentType;
class Element;
class NodeList;
class ProcessingInstruction;
class Text;

class Document : public ::jxx::lang::InterfaceBase<Document, Node> {
public:
    ~Document() override = default;

    virtual ::jxx::Ptr<DocumentType> getDoctype() const = 0;
    virtual ::jxx::Ptr<DOMImplementation> getImplementation() const = 0;
    virtual ::jxx::Ptr<Element> getDocumentElement() const = 0;

    virtual ::jxx::Ptr<Element> createElement(
        const ::jxx::Ptr<::jxx::lang::String>& tagName) = 0;

    virtual ::jxx::Ptr<Text> createTextNode(
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;

    virtual ::jxx::Ptr<Comment> createComment(
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;

    virtual ::jxx::Ptr<CDATASection> createCDATASection(
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;

    virtual ::jxx::Ptr<ProcessingInstruction>
    createProcessingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;

    virtual ::jxx::Ptr<Attr> createAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;

    virtual ::jxx::Ptr<NodeList> getElementsByTagName(
        const ::jxx::Ptr<::jxx::lang::String>& tagName) const = 0;

    virtual ::jxx::Ptr<Element> createElementNS(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) = 0;

    virtual ::jxx::Ptr<Attr> createAttributeNS(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) = 0;

    virtual ::jxx::Ptr<NodeList> getElementsByTagNameNS(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;

    virtual ::jxx::Ptr<Element> getElementById(
        const ::jxx::Ptr<::jxx::lang::String>& elementId) const = 0;

    virtual ::jxx::Ptr<Node> importNode(
        const ::jxx::Ptr<Node>& importedNode,
        ::jxx::lang::jbool deep) = 0;

    virtual ::jxx::Ptr<Node> adoptNode(
        const ::jxx::Ptr<Node>& source) = 0;

    virtual ::jxx::Ptr<Node> renameNode(
        const ::jxx::Ptr<Node>& node,
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getInputEncoding() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getXmlEncoding() const = 0;
    virtual ::jxx::lang::jbool getXmlStandalone() const = 0;
    virtual void setXmlStandalone(::jxx::lang::jbool standalone) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getXmlVersion() const = 0;
    virtual void setXmlVersion(const ::jxx::Ptr<::jxx::lang::String>& version) = 0;
    virtual ::jxx::lang::jbool getStrictErrorChecking() const = 0;
    virtual void setStrictErrorChecking(::jxx::lang::jbool strict) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getDocumentURI() const = 0;
    virtual void setDocumentURI(const ::jxx::Ptr<::jxx::lang::String>& uri) = 0;
    virtual void normalizeDocument() = 0;
};

} // namespace jxx::org::w3c::dom
