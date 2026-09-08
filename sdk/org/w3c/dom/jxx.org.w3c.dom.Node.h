#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

/*
 * Some platform XML headers expose DOM node constants as preprocessor
 * macros. Remove those macros so the JXX Node interface can expose the
 * required public constant names.
 */
#ifdef ELEMENT_NODE
#undef ELEMENT_NODE
#endif

#ifdef ATTRIBUTE_NODE
#undef ATTRIBUTE_NODE
#endif

#ifdef TEXT_NODE
#undef TEXT_NODE
#endif

#ifdef CDATA_SECTION_NODE
#undef CDATA_SECTION_NODE
#endif

#ifdef ENTITY_REFERENCE_NODE
#undef ENTITY_REFERENCE_NODE
#endif

#ifdef ENTITY_NODE
#undef ENTITY_NODE
#endif

#ifdef PROCESSING_INSTRUCTION_NODE
#undef PROCESSING_INSTRUCTION_NODE
#endif

#ifdef COMMENT_NODE
#undef COMMENT_NODE
#endif

#ifdef DOCUMENT_NODE
#undef DOCUMENT_NODE
#endif

#ifdef DOCUMENT_TYPE_NODE
#undef DOCUMENT_TYPE_NODE
#endif

#ifdef DOCUMENT_FRAGMENT_NODE
#undef DOCUMENT_FRAGMENT_NODE
#endif

#ifdef NOTATION_NODE
#undef NOTATION_NODE
#endif

#ifdef DOCUMENT_POSITION_DISCONNECTED
#undef DOCUMENT_POSITION_DISCONNECTED
#endif

#ifdef DOCUMENT_POSITION_PRECEDING
#undef DOCUMENT_POSITION_PRECEDING
#endif

#ifdef DOCUMENT_POSITION_FOLLOWING
#undef DOCUMENT_POSITION_FOLLOWING
#endif

#ifdef DOCUMENT_POSITION_CONTAINS
#undef DOCUMENT_POSITION_CONTAINS
#endif

#ifdef DOCUMENT_POSITION_CONTAINED_BY
#undef DOCUMENT_POSITION_CONTAINED_BY
#endif

#ifdef DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC
#undef DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC
#endif

namespace jxx::org::w3c::dom
{

    class Document;
    class NamedNodeMap;
    class NodeList;

    class Node
        : public jxx::lang::InterfaceBase<Node>
    {
    public:
        enum : jxx::lang::jshort
        {
            ELEMENT_NODE = 1,
            ATTRIBUTE_NODE = 2,
            TEXT_NODE = 3,
            CDATA_SECTION_NODE = 4,
            ENTITY_REFERENCE_NODE = 5,
            ENTITY_NODE = 6,
            PROCESSING_INSTRUCTION_NODE = 7,
            COMMENT_NODE = 8,
            DOCUMENT_NODE = 9,
            DOCUMENT_TYPE_NODE = 10,
            DOCUMENT_FRAGMENT_NODE = 11,
            NOTATION_NODE = 12
        };

        enum : jxx::lang::jshort
        {
            DOCUMENT_POSITION_DISCONNECTED = 0x01,
            DOCUMENT_POSITION_PRECEDING = 0x02,
            DOCUMENT_POSITION_FOLLOWING = 0x04,
            DOCUMENT_POSITION_CONTAINS = 0x08,
            DOCUMENT_POSITION_CONTAINED_BY = 0x10,
            DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 0x20
        };

        ~Node() override = default;

        virtual jxx::Ptr<jxx::lang::String>
            getNodeName() const = 0;

        virtual jxx::Ptr<jxx::lang::String>
            getNodeValue() const = 0;

        virtual void setNodeValue(
            const jxx::Ptr<jxx::lang::String>& value) = 0;

        virtual jxx::lang::jshort
            getNodeType() const = 0;

        virtual jxx::Ptr<Node>
            getParentNode() const = 0;

        virtual jxx::Ptr<NodeList>
            getChildNodes() const = 0;

        virtual jxx::Ptr<Node>
            getFirstChild() const = 0;

        virtual jxx::Ptr<Node>
            getLastChild() const = 0;

        virtual jxx::Ptr<Node>
            getPreviousSibling() const = 0;

        virtual jxx::Ptr<Node>
            getNextSibling() const = 0;

        virtual jxx::Ptr<NamedNodeMap>
            getAttributes() const = 0;

        virtual jxx::Ptr<Document>
            getOwnerDocument() const = 0;

        virtual jxx::Ptr<Node> insertBefore(
            const jxx::Ptr<Node>& child,
            const jxx::Ptr<Node>& reference) = 0;

        virtual jxx::Ptr<Node> replaceChild(
            const jxx::Ptr<Node>& child,
            const jxx::Ptr<Node>& oldChild) = 0;

        virtual jxx::Ptr<Node> removeChild(
            const jxx::Ptr<Node>& child) = 0;

        virtual jxx::Ptr<Node> appendChild(
            const jxx::Ptr<Node>& child) = 0;

        virtual jxx::lang::jbool
            hasChildNodes() const = 0;

        virtual jxx::Ptr<Node> cloneNode(
            jxx::lang::jbool deep) const = 0;

        virtual void normalize() = 0;

        virtual jxx::lang::jbool
            hasAttributes() const = 0;

        virtual jxx::Ptr<jxx::lang::String>
            getNamespaceURI() const = 0;

        virtual jxx::Ptr<jxx::lang::String>
            getPrefix() const = 0;

        virtual void setPrefix(
            const jxx::Ptr<jxx::lang::String>& prefix) = 0;

        virtual jxx::Ptr<jxx::lang::String>
            getLocalName() const = 0;

        virtual jxx::Ptr<jxx::lang::String>
            getTextContent() const = 0;

        virtual void setTextContent(
            const jxx::Ptr<jxx::lang::String>& text) = 0;

        virtual jxx::lang::jbool isSameNode(
            const jxx::Ptr<Node>& other) const = 0;
    };

} // namespace jxx::org::w3c::dom
