#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <pugixml.hpp>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"

namespace jxx::org::w3c::dom::internal {

using String = ::jxx::lang::String;
using Node = ::jxx::org::w3c::dom::Node;
using Document = ::jxx::org::w3c::dom::Document;
using Element = ::jxx::org::w3c::dom::Element;
using Attr = ::jxx::org::w3c::dom::Attr;
using Text = ::jxx::org::w3c::dom::Text;
using NodeList = ::jxx::org::w3c::dom::NodeList;
using NamedNodeMap = ::jxx::org::w3c::dom::NamedNodeMap;
using DOMImplementation = ::jxx::org::w3c::dom::DOMImplementation;
using DOMException = ::jxx::org::w3c::dom::DOMException;

struct Store {
    pugi::xml_document document;
};

class DomNode;

::jxx::Ptr<Document> newDocument();

::jxx::Ptr<Node> wrap(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node);

class NodeListImpl final
    : public ::jxx::lang::ClassBase<
          NodeListImpl,
          ::jxx::lang::Object,
          NodeList> {
public:
    NodeListImpl(
        std::shared_ptr<Store> store,
        std::vector<pugi::xml_node> nodes)
        : store_(std::move(store))
        , nodes_(std::move(nodes)) {
    }

    ::jxx::Ptr<Node> item(
        ::jxx::lang::jint index) const override {

        if (index < 0 ||
            static_cast<std::size_t>(index) >= nodes_.size()) {
            return nullptr;
        }

        return wrap(
            store_,
            nodes_[static_cast<std::size_t>(index)]);
    }

    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(nodes_.size());
    }

private:
    std::shared_ptr<Store> store_;
    std::vector<pugi::xml_node> nodes_;
};

class DomNode final
    : public ::jxx::lang::ClassBase<
          DomNode,
          ::jxx::lang::Object,
          Document,
          Element,
          Attr,
          Text> {
public:
    DomNode(
        std::shared_ptr<Store> store,
        pugi::xml_node node)
        : store_(std::move(store))
        , node_(node) {
    }

    ::jxx::Ptr<String> getNodeName() const override {
        return ::jxx::NEW<String>(node_.name());
    }

    ::jxx::Ptr<String> getNodeValue() const override {
        return ::jxx::NEW<String>(node_.value());
    }

    void setNodeValue(
        const ::jxx::Ptr<String>& value) override {
        node_.set_value(value ? value->utf8().c_str() : "");
    }

    ::jxx::lang::jshort getNodeType() const override {
        switch (node_.type()) {
        case pugi::node_document: return Node::DOCUMENT_NODE;
        case pugi::node_element: return Node::ELEMENT_NODE;
        case pugi::node_pcdata: return Node::TEXT_NODE;
        case pugi::node_cdata: return Node::CDATA_SECTION_NODE;
        case pugi::node_comment: return Node::COMMENT_NODE;
        case pugi::node_pi: return Node::PROCESSING_INSTRUCTION_NODE;
        default: return 0;
        }
    }

    ::jxx::Ptr<Node> getParentNode() const override {
        return wrap(store_, node_.parent());
    }

    ::jxx::Ptr<NodeList> getChildNodes() const override {
        std::vector<pugi::xml_node> nodes;
        for (const auto& node : node_.children()) {
            nodes.push_back(node);
        }
        return ::jxx::NEW<NodeListImpl>(store_, std::move(nodes));
    }

    ::jxx::Ptr<Node> getFirstChild() const override {
        return wrap(store_, node_.first_child());
    }

    ::jxx::Ptr<Node> getLastChild() const override {
        return wrap(store_, node_.last_child());
    }

    ::jxx::Ptr<Node> getPreviousSibling() const override {
        return wrap(store_, node_.previous_sibling());
    }

    ::jxx::Ptr<Node> getNextSibling() const override {
        return wrap(store_, node_.next_sibling());
    }

    ::jxx::Ptr<NamedNodeMap> getAttributes() const override;

    ::jxx::Ptr<Document> getOwnerDocument() const override {
        if (node_.type() == pugi::node_document) {
            return nullptr;
        }

        return ::jxx::CAST<Document>(
            wrap(store_, store_->document));
    }

    ::jxx::Ptr<Node> insertBefore(
        const ::jxx::Ptr<Node>& child,
        const ::jxx::Ptr<Node>& reference) override;

    ::jxx::Ptr<Node> replaceChild(
        const ::jxx::Ptr<Node>& child,
        const ::jxx::Ptr<Node>& oldChild) override;

    ::jxx::Ptr<Node> removeChild(
        const ::jxx::Ptr<Node>& child) override;

    ::jxx::Ptr<Node> appendChild(
        const ::jxx::Ptr<Node>& child) override;

    ::jxx::lang::jbool hasChildNodes() const override {
        return static_cast<::jxx::lang::jbool>(
            static_cast<bool>(node_.first_child()));
    }

    ::jxx::Ptr<Node> cloneNode(
        ::jxx::lang::jbool deep) const override {
        auto document = newDocument();
        auto target = ::jxx::CAST<DomNode>(document);
        auto copied = target->node_.append_copy(node_);
        if (!deep) {
            while (copied.first_child()) {
                copied.remove_child(copied.first_child());
            }
        }
        return wrap(target->store_, copied);
    }

    void normalize() override {
    }

    ::jxx::lang::jbool hasAttributes() const override {
        return static_cast<::jxx::lang::jbool>(
            static_cast<bool>(node_.first_attribute()));
    }

    ::jxx::Ptr<String> getNamespaceURI() const override {
        return nullptr;
    }

    ::jxx::Ptr<String> getPrefix() const override {
        const std::string name = node_.name();
        const auto separator = name.find(':');
        return separator == std::string::npos
            ? nullptr
            : ::jxx::NEW<String>(name.substr(0, separator));
    }

    void setPrefix(
        const ::jxx::Ptr<String>&) override {
        throw DOMException(
            DOMException::NOT_SUPPORTED_ERR,
            ::jxx::NEW<String>("Prefix mutation is not supported"));
    }

    ::jxx::Ptr<String> getLocalName() const override {
        const std::string name = node_.name();
        const auto separator = name.find(':');
        return ::jxx::NEW<String>(
            separator == std::string::npos
                ? name
                : name.substr(separator + 1));
    }

    ::jxx::Ptr<String> getTextContent() const override {
        return ::jxx::NEW<String>(node_.text().get());
    }

    void setTextContent(
        const ::jxx::Ptr<String>& text) override {
        node_.text().set(text ? text->utf8().c_str() : "");
    }

    ::jxx::lang::jbool isSameNode(
        const ::jxx::Ptr<Node>& other) const override {
        const auto value = ::jxx::CAST<DomNode>(other);
        return static_cast<::jxx::lang::jbool>(
            value && value->store_ == store_ && value->node_ == node_);
    }

    ::jxx::Ptr<Element> getDocumentElement() const override {
        if (node_.type() != pugi::node_document) {
            return nullptr;
        }

        return ::jxx::CAST<Element>(
            wrap(
                store_,
                store_->document.document_element()));
    }

    ::jxx::Ptr<Element> createElement(
        const ::jxx::Ptr<String>& name) override {
        auto value = store_->document.append_child(name->utf8().c_str());
        return ::jxx::CAST<Element>(wrap(store_, value));
    }

    ::jxx::Ptr<Text> createTextNode(
        const ::jxx::Ptr<String>& data) override {
        auto value = store_->document.append_child(pugi::node_pcdata);
        value.set_value(data ? data->utf8().c_str() : "");
        return ::jxx::CAST<Text>(wrap(store_, value));
    }

    ::jxx::Ptr<Attr> createAttribute(
        const ::jxx::Ptr<String>&) override {
        return nullptr;
    }

    ::jxx::Ptr<NodeList> getElementsByTagName(
        const ::jxx::Ptr<String>& name) const override {
        std::vector<pugi::xml_node> nodes;
        const std::string query = name->utf8();
        std::function<void(pugi::xml_node)> scan =
            [&](pugi::xml_node current) {
                for (const auto& child : current.children()) {
                    if (query == "*" || query == child.name()) {
                        nodes.push_back(child);
                    }
                    scan(child);
                }
            };
        scan(node_);
        return ::jxx::NEW<NodeListImpl>(store_, std::move(nodes));
    }

    ::jxx::Ptr<DOMImplementation> getImplementation() const override {
        return ::jxx::org::w3c::dom::internal::implementation();
    }

    ::jxx::Ptr<String> getTagName() const override {
        return getNodeName();
    }

    ::jxx::Ptr<String> getAttribute(
        const ::jxx::Ptr<String>& name) const override {
        return ::jxx::NEW<String>(
            node_.attribute(name->utf8().c_str()).value());
    }

    void setAttribute(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<String>& value) override {
        auto attribute = node_.attribute(name->utf8().c_str());
        if (!attribute) {
            attribute = node_.append_attribute(name->utf8().c_str());
        }
        attribute.set_value(value ? value->utf8().c_str() : "");
    }

    void removeAttribute(
        const ::jxx::Ptr<String>& name) override {
        node_.remove_attribute(name->utf8().c_str());
    }

    ::jxx::lang::jbool hasAttribute(
        const ::jxx::Ptr<String>& name) const override {
        return static_cast<::jxx::lang::jbool>(
            static_cast<bool>(node_.attribute(name->utf8().c_str())));
    }

    ::jxx::Ptr<String> getName() const override {
        return getNodeName();
    }

    ::jxx::Ptr<String> getValue() const override {
        return getNodeValue();
    }

    void setValue(
        const ::jxx::Ptr<String>& value) override {
        setNodeValue(value);
    }

    ::jxx::Ptr<String> getData() const override {
        return getNodeValue();
    }

    void setData(
        const ::jxx::Ptr<String>& value) override {
        setNodeValue(value);
    }

    ::jxx::lang::jint getLength() const override {
        const auto value =
            ::jxx::NEW<String>(node_.value());

        return value->length();
    }

    ::jxx::Ptr<String> substringData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count) const override {
        const std::string value = node_.value();
        if (offset < 0 || count < 0 ||
            static_cast<std::size_t>(offset) > value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>("Invalid character-data range"));
        }
        return ::jxx::NEW<String>(
            value.substr(
                static_cast<std::size_t>(offset),
                static_cast<std::size_t>(count)));
    }

    void appendData(
        const ::jxx::Ptr<String>& data) override {
        std::string value = node_.value();
        if (data) value += data->utf8();
        node_.set_value(value.c_str());
    }

    void insertData(
        ::jxx::lang::jint offset,
        const ::jxx::Ptr<String>& data) override {
        std::string value = node_.value();
        if (offset < 0 || static_cast<std::size_t>(offset) > value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>("Invalid character-data offset"));
        }
        value.insert(
            static_cast<std::size_t>(offset),
            data ? data->utf8() : std::string());
        node_.set_value(value.c_str());
    }

    void deleteData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count) override {
        std::string value = node_.value();
        if (offset < 0 || count < 0 ||
            static_cast<std::size_t>(offset) > value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>("Invalid character-data range"));
        }
        value.erase(
            static_cast<std::size_t>(offset),
            static_cast<std::size_t>(count));
        node_.set_value(value.c_str());
    }

    void replaceData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count,
        const ::jxx::Ptr<String>& data) override {
        std::string value = node_.value();
        if (offset < 0 || count < 0 ||
            static_cast<std::size_t>(offset) > value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>("Invalid character-data range"));
        }
        value.replace(
            static_cast<std::size_t>(offset),
            static_cast<std::size_t>(count),
            data ? data->utf8() : std::string());
        node_.set_value(value.c_str());
    }

    ::jxx::Ptr<Text> splitText(
        ::jxx::lang::jint offset) override {
        std::string value = node_.value();
        if (offset < 0 || static_cast<std::size_t>(offset) > value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>("Invalid text offset"));
        }
        const std::string tail = value.substr(static_cast<std::size_t>(offset));
        value.erase(static_cast<std::size_t>(offset));
        node_.set_value(value.c_str());
        auto next = node_.parent().insert_child_after(pugi::node_pcdata, node_);
        next.set_value(tail.c_str());
        return ::jxx::CAST<Text>(wrap(store_, next));
    }

    ::jxx::lang::jbool isElementContentWhitespace() const override {
        return false;
    }

    ::jxx::Ptr<String> getWholeText() const override {
        return getData();
    }

    ::jxx::Ptr<Text> replaceWholeText(
        const ::jxx::Ptr<String>& content) override {
        setData(content);
        return ::jxx::CAST<Text>(thisPtr());
    }

    std::shared_ptr<Store> store_;
    pugi::xml_node node_;
};

class NamedNodeMapImpl final
    : public ::jxx::lang::ClassBase<
          NamedNodeMapImpl,
          ::jxx::lang::Object,
          NamedNodeMap> {
public:
    NamedNodeMapImpl(
        std::shared_ptr<Store> store,
        pugi::xml_node node)
        : store_(std::move(store))
        , node_(node) {
    }

    ::jxx::Ptr<Node> getNamedItem(
        const ::jxx::Ptr<String>&) const override {
        return nullptr;
    }

    ::jxx::Ptr<Node> setNamedItem(
        const ::jxx::Ptr<Node>&) override {
        throw DOMException(
            DOMException::NOT_SUPPORTED_ERR,
            ::jxx::NEW<String>("Attribute nodes are not mutable"));
    }

    ::jxx::Ptr<Node> removeNamedItem(
        const ::jxx::Ptr<String>& name) override {
        node_.remove_attribute(name->utf8().c_str());
        return nullptr;
    }

    ::jxx::Ptr<Node> item(
        ::jxx::lang::jint) const override {
        return nullptr;
    }

    ::jxx::lang::jint getLength() const override {
        ::jxx::lang::jint count = 0;
        for (const auto& attribute : node_.attributes()) {
            (void)attribute;
            ++count;
        }
        return count;
    }

private:
    std::shared_ptr<Store> store_;
    pugi::xml_node node_;
};

::jxx::Ptr<NamedNodeMap> DomNode::getAttributes() const {
    return ::jxx::NEW<NamedNodeMapImpl>(store_, node_);
}

::jxx::Ptr<Node> DomNode::appendChild(
    const ::jxx::Ptr<Node>& child) {
    const auto value = ::jxx::CAST<DomNode>(child);
    if (!value) {
        throw DOMException(
            DOMException::WRONG_DOCUMENT_ERR,
            ::jxx::NEW<String>("Unsupported node"));
    }
    return wrap(store_, node_.append_copy(value->node_));
}

::jxx::Ptr<Node> DomNode::insertBefore(
    const ::jxx::Ptr<Node>& child,
    const ::jxx::Ptr<Node>& reference) {
    const auto value = ::jxx::CAST<DomNode>(child);
    const auto before = ::jxx::CAST<DomNode>(reference);
    if (!value || !before) {
        throw DOMException(
            DOMException::NOT_FOUND_ERR,
            ::jxx::NEW<String>("Invalid child or reference"));
    }
    return wrap(
        store_,
        node_.insert_copy_before(value->node_, before->node_));
}

::jxx::Ptr<Node> DomNode::replaceChild(
    const ::jxx::Ptr<Node>& child,
    const ::jxx::Ptr<Node>& oldChild) {
    auto result = insertBefore(child, oldChild);
    removeChild(oldChild);
    return result;
}

::jxx::Ptr<Node> DomNode::removeChild(
    const ::jxx::Ptr<Node>& child) {
    const auto value = ::jxx::CAST<DomNode>(child);
    if (!value || !node_.remove_child(value->node_)) {
        throw DOMException(
            DOMException::NOT_FOUND_ERR,
            ::jxx::NEW<String>("Child not found"));
    }
    return child;
}

::jxx::Ptr<Node> wrap(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node) {
    return node
        ? ::jxx::CAST<Node>(::jxx::NEW<DomNode>(store, node))
        : nullptr;
}

::jxx::Ptr<Document> parseDocument(
    const std::string& xml,
    ::jxx::lang::jbool ignoreComments,
    ::jxx::lang::jbool coalescing,
    std::string& error,
    ::jxx::lang::jint& offset) {
    auto store = std::make_shared<Store>();
    unsigned int flags = pugi::parse_default;
    if (!ignoreComments) flags |= pugi::parse_comments;
    if (!coalescing) flags |= pugi::parse_cdata;
    const auto result = store->document.load_buffer(
        xml.data(), xml.size(), flags, pugi::encoding_auto);
    if (!result) {
        error = result.description();
        offset = static_cast<::jxx::lang::jint>(result.offset);
        return nullptr;
    }
    return ::jxx::CAST<Document>(wrap(store, store->document));
}

::jxx::Ptr<Document> loadDocument(
    const std::string& path,
    ::jxx::lang::jbool ignoreComments,
    ::jxx::lang::jbool coalescing,
    std::string& error,
    ::jxx::lang::jint& offset) {
    auto store = std::make_shared<Store>();
    unsigned int flags = pugi::parse_default;
    if (!ignoreComments) flags |= pugi::parse_comments;
    if (!coalescing) flags |= pugi::parse_cdata;
    const auto result = store->document.load_file(
        path.c_str(), flags, pugi::encoding_auto);
    if (!result) {
        error = result.description();
        offset = static_cast<::jxx::lang::jint>(result.offset);
        return nullptr;
    }
    return ::jxx::CAST<Document>(wrap(store, store->document));
}

::jxx::Ptr<Document> newDocument() {
    auto store = std::make_shared<Store>();
    return ::jxx::CAST<Document>(wrap(store, store->document));
}

} // namespace jxx::org::w3c::dom::internal
