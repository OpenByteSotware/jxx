#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"

#include <algorithm>
#include <functional>
#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

#include <pugixml.hpp>
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CDATASection.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Comment.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DocumentType.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ProcessingInstruction.h"
#include "org/w3c/dom/jxx.org.w3c.dom.UserDataHandler.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "org/w3c/dom/jxx.org.w3c.dom.TypeInfo.h"

namespace jxx::org::w3c::dom::internal {

using String = ::jxx::lang::String;
using Node = ::jxx::org::w3c::dom::Node;
using Document = ::jxx::org::w3c::dom::Document;
using Element = ::jxx::org::w3c::dom::Element;
using Attr = ::jxx::org::w3c::dom::Attr;
using CDATASection = ::jxx::org::w3c::dom::CDATASection;
using Comment = ::jxx::org::w3c::dom::Comment;
using Text = ::jxx::org::w3c::dom::Text;
using NodeList = ::jxx::org::w3c::dom::NodeList;
using NamedNodeMap = ::jxx::org::w3c::dom::NamedNodeMap;
using ProcessingInstruction = ::jxx::org::w3c::dom::ProcessingInstruction;
using DOMImplementation = ::jxx::org::w3c::dom::DOMImplementation;
using DOMException = ::jxx::org::w3c::dom::DOMException;
using DOMConfiguration = ::jxx::org::w3c::dom::DOMConfiguration;
using DOMStringList = ::jxx::org::w3c::dom::DOMStringList;
using UserDataHandler = ::jxx::org::w3c::dom::UserDataHandler;

struct UserDataEntry {
    ::jxx::Ptr<::jxx::lang::Object> data;
    ::jxx::Ptr<UserDataHandler> handler;
};

struct Store {
    pugi::xml_document document;
    std::unordered_set<std::string> idAttributes;
    std::unordered_map<
        std::size_t,
        std::unordered_map<std::string, UserDataEntry>> userData;
    ::jxx::Ptr<String> inputEncoding;
    ::jxx::Ptr<String> xmlEncoding;
    ::jxx::Ptr<String> xmlVersion = ::jxx::NEW<String>("1.0");
    ::jxx::Ptr<String> documentURI;
    ::jxx::lang::jbool xmlStandalone = false;
    ::jxx::lang::jbool strictErrorChecking = true;
    std::unordered_map<std::string, ::jxx::lang::jbool> domBooleanParameters{
        {"canonical-form", false},
        {"cdata-sections", true},
        {"comments", true},
        {"element-content-whitespace", true},
        {"entities", true},
        {"namespace-declarations", true},
        {"namespaces", true},
        {"normalize-characters", false},
        {"split-cdata-sections", true},
        {"validate", false},
        {"validate-if-schema", false},
        {"well-formed", true}
    };
};

std::string localPart(const std::string& qualifiedName) {
    const auto position = qualifiedName.find(':');
    return position == std::string::npos ? qualifiedName : qualifiedName.substr(position + 1);
}

std::string prefixPart(const std::string& qualifiedName) {
    const auto position = qualifiedName.find(':');
    return position == std::string::npos ? std::string() : qualifiedName.substr(0, position);
}

std::u16string utf16Value(
    std::u16string result;

    if (value == nullptr) {
        return result;
    }

    const auto length = value->length();
    result.reserve(static_cast<std::size_t>(length));

    for (::jxx::lang::jint index = 0;
         index < length;
         ++index) {
        result.push_back(
            static_cast<char16_t>(
                value->charAt(index)));
    }

    return result;
}

void setNativeCharacterData(
    pugi::xml_node node,
    const std::u16string& value) {
    const auto converted =
        ::jxx::NEW<String>(value);

    node.set_value(
        converted->utf8().c_str());
}

std::string resolveNamespace(pugi::xml_node node, const std::string& qualifiedName) {
    const auto prefix = prefixPart(qualifiedName);
    const std::string declaration = prefix.empty() ? "xmlns" : "xmlns:" + prefix;
    for (auto current = node; current; current = current.parent()) {
        const auto value = current.attribute(declaration.c_str());
        if (value) return value.value();
    }
    return {};
}

pugi::xml_attribute findAttributeNS(pugi::xml_node node, const std::string& uri, const std::string& local) {
    for (const auto& attribute : node.attributes()) {
        const std::string name = attribute.name();
        if (name == "xmlns" || name.rfind("xmlns:", 0) == 0) continue;
        if (localPart(name) == local && resolveNamespace(node, name) == uri) return attribute;
    }
    return {};
}

std::size_t userDataNodeKey(
    pugi::xml_node node,
    pugi::xml_attribute attribute,
    bool attributeNode) {
    if (attributeNode) {
        return attribute.hash_value() ^
            (node.hash_value() + static_cast<std::size_t>(0x9e3779b9U));
    }
    return node.hash_value();
}

void notifyUserDataHandlers(
    const std::shared_ptr<Store>& store,
    std::size_t nodeKey,
    ::jxx::lang::jshort operation,
    const ::jxx::Ptr<Node>& source,
    const ::jxx::Ptr<Node>& destination) {
    const auto values = store->userData.find(nodeKey);
    if (values == store->userData.end()) {
        return;
    }

    const auto snapshot = values->second;
    for (const auto& entry : snapshot) {
        if (entry.second.handler == nullptr) {
            continue;
        }

        try {
            entry.second.handler->handle(
                operation,
                ::jxx::NEW<String>(entry.first),
                entry.second.data,
                source,
                destination);
        }
        catch (...) {
            // User-data callbacks must not interrupt the DOM operation.
        }
    }
}

class DOMStringListImpl final
    : public ::jxx::lang::ClassBase<
          DOMStringListImpl,
          ::jxx::lang::Object,
          DOMStringList> {
public:
    explicit DOMStringListImpl(std::vector<std::string> values)
        : values_(std::move(values)) {
    }

    ::jxx::Ptr<String> item(::jxx::lang::jint index) const override {
        if (index < 0 || static_cast<std::size_t>(index) >= values_.size()) {
            return nullptr;
        }
        return ::jxx::NEW<String>(values_[static_cast<std::size_t>(index)]);
    }

    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }

    ::jxx::lang::jbool contains(
        const ::jxx::Ptr<String>& value) const override {
        if (value == nullptr) {
            return false;
        }
        return std::find(values_.begin(), values_.end(), value->utf8()) !=
            values_.end();
    }

private:
    std::vector<std::string> values_;
};

class DOMConfigurationImpl final
    : public ::jxx::lang::ClassBase<
          DOMConfigurationImpl,
          ::jxx::lang::Object,
          DOMConfiguration> {
public:
    explicit DOMConfigurationImpl(const std::shared_ptr<Store>& store)
        : store_(store) {
    }

    void setParameter(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        const auto normalized = normalizeName_(name);
        const auto found = store_->domBooleanParameters.find(normalized);
        if (found == store_->domBooleanParameters.end()) {
            throw DOMException(
                DOMException::NOT_FOUND_ERR,
                ::jxx::NEW<String>("Unrecognized DOM configuration parameter"));
        }

        const auto booleanValue = ::jxx::CAST<::jxx::lang::Boolean>(value);
        if (booleanValue == nullptr) {
            throw DOMException(
                DOMException::TYPE_MISMATCH_ERR,
                ::jxx::NEW<String>("DOM configuration parameter requires Boolean"));
        }

        const auto requested = booleanValue->booleanValue();
        if (!canSetParameter(name, value)) {
            throw DOMException(
                DOMException::NOT_SUPPORTED_ERR,
                ::jxx::NEW<String>("Unsupported DOM configuration value"));
        }
        found->second = requested;
    }

    ::jxx::Ptr<::jxx::lang::Object> getParameter(
        const ::jxx::Ptr<String>& name) const override {
        const auto found = store_->domBooleanParameters.find(normalizeName_(name));
        if (found == store_->domBooleanParameters.end()) {
            throw DOMException(
                DOMException::NOT_FOUND_ERR,
                ::jxx::NEW<String>("Unrecognized DOM configuration parameter"));
        }
        return ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Boolean::valueOf(found->second));
    }

    ::jxx::lang::jbool canSetParameter(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) const override {
        const auto normalized = normalizeName_(name);
        const auto found = store_->domBooleanParameters.find(normalized);
        if (found == store_->domBooleanParameters.end()) {
            return false;
        }
        const auto booleanValue = ::jxx::CAST<::jxx::lang::Boolean>(value);
        if (booleanValue == nullptr) {
            return false;
        }
        const auto requested = booleanValue->booleanValue();
        if (requested &&
            (normalized == "canonical-form" ||
             normalized == "normalize-characters" ||
             normalized == "validate" ||
             normalized == "validate-if-schema")) {
            return false;
        }
        return true;
    }

    ::jxx::Ptr<DOMStringList> getParameterNames() const override {
        std::vector<std::string> names;
        names.reserve(store_->domBooleanParameters.size());
        for (const auto& entry : store_->domBooleanParameters) {
            names.push_back(entry.first);
        }
        std::sort(names.begin(), names.end());
        return ::jxx::NEW<DOMStringListImpl>(std::move(names));
    }

private:
    static std::string normalizeName_(const ::jxx::Ptr<String>& name) {
        if (name == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        auto value = name->utf8();
        std::transform(
            value.begin(), value.end(), value.begin(),
            [](unsigned char character) {
                return static_cast<char>(std::tolower(character));
            });
        return value;
    }

    std::shared_ptr<Store> store_;
};

class DomNode;
class NamedNodeMapImpl;

::jxx::Ptr<Document> newDocument();

::jxx::Ptr<Node> wrap(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node);

::jxx::Ptr<Comment> wrapComment(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node);

::jxx::Ptr<CDATASection> wrapCDATASection(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node);

::jxx::Ptr<ProcessingInstruction> wrapProcessingInstruction(
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

class DomNode
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

    DomNode(
        std::shared_ptr<Store> store,
        pugi::xml_node owner,
        pugi::xml_attribute attribute)
        : store_(std::move(store))
        , node_(owner)
        , attribute_(attribute)
        , attributeNode_(true) {
    }

    ::jxx::Ptr<String> getNodeName() const override {
        return ::jxx::NEW<String>(
            attributeNode_ ? attribute_.name() : node_.name());
    }

    ::jxx::Ptr<String> getNodeValue() const override {
        return ::jxx::NEW<String>(
            attributeNode_ ? attribute_.value() : node_.value());
    }

    void setNodeValue(
        const ::jxx::Ptr<String>& value) override {
        if (attributeNode_) {
            attribute_.set_value(value ? value->utf8().c_str() : "");
            return;
        }
        node_.set_value(value ? value->utf8().c_str() : "");
    }

    ::jxx::lang::jshort getNodeType() const override {
        if (attributeNode_) {
            return Node::ATTRIBUTE_NODE;
        }
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
        if (attributeNode_) {
            return nullptr;
        }
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

        const auto destination = wrap(target->store_, copied);
        notifyUserDataHandlers(
            store_,
            userDataNodeKey(node_, attribute_, attributeNode_),
            UserDataHandler::NODE_CLONED,
            ::jxx::CAST<Node>(
                const_cast<DomNode*>(this)->thisPtr()),
            destination);
        return destination;
    }

    void normalize() override {
    }

    ::jxx::lang::jbool hasAttributes() const override {
        return static_cast<::jxx::lang::jbool>(
            static_cast<bool>(node_.first_attribute()));
    }

    ::jxx::Ptr<String> getNamespaceURI() const override {
        const std::string qualifiedName =
            attributeNode_
                ? std::string(attribute_.name())
                : std::string(node_.name());

        if (qualifiedName.empty()) {
            return nullptr;
        }

        const std::string prefix = prefixPart(qualifiedName);

        if (qualifiedName == "xmlns" || prefix == "xmlns") {
            return ::jxx::NEW<String>(
                "http://www.w3.org/2000/xmlns/");
        }

        if (prefix == "xml") {
            return ::jxx::NEW<String>(
                "http://www.w3.org/XML/1998/namespace");
        }

        if (attributeNode_ && prefix.empty()) {
            return nullptr;
        }

        const std::string namespaceURI =
            resolveNamespace(node_, qualifiedName);

        return namespaceURI.empty()
            ? nullptr
            : ::jxx::NEW<String>(namespaceURI);
    }

    ::jxx::Ptr<String> getPrefix() const override {
        const std::string qualifiedName =
            attributeNode_
                ? std::string(attribute_.name())
                : std::string(node_.name());
        const std::string prefix = prefixPart(qualifiedName);
        return prefix.empty() ? nullptr : ::jxx::NEW<String>(prefix);
    }

    void setPrefix(
        const ::jxx::Ptr<String>&) override {
        throw DOMException(
            DOMException::NOT_SUPPORTED_ERR,
            ::jxx::NEW<String>("Prefix mutation is not supported"));
    }

    ::jxx::Ptr<String> getLocalName() const override {
        const std::string qualifiedName =
            attributeNode_
                ? std::string(attribute_.name())
                : std::string(node_.name());
        return qualifiedName.empty()
            ? nullptr
            : ::jxx::NEW<String>(localPart(qualifiedName));
    }

    ::jxx::Ptr<String> getTextContent() const override {
        return ::jxx::NEW<String>(node_.text().get());
    }

    void setTextContent(
        const ::jxx::Ptr<String>& text) override {
        node_.text().set(text ? text->utf8().c_str() : "");
    }

    ::jxx::Ptr<String> getBaseURI() const override {
        return store_->documentURI;
    }

    ::jxx::lang::jshort compareDocumentPosition(
        const ::jxx::Ptr<Node>& other) const override {
        const auto value = ::jxx::CAST<DomNode>(other);
        if (!value || value->store_ != store_) {
            return Node::DOCUMENT_POSITION_DISCONNECTED |
                Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC;
        }
        if (value->node_ == node_) return 0;
        for (auto parent = value->node_.parent(); parent; parent = parent.parent()) {
            if (parent == node_) return Node::DOCUMENT_POSITION_CONTAINED_BY |
                Node::DOCUMENT_POSITION_FOLLOWING;
        }
        for (auto parent = node_.parent(); parent; parent = parent.parent()) {
            if (parent == value->node_) return Node::DOCUMENT_POSITION_CONTAINS |
                Node::DOCUMENT_POSITION_PRECEDING;
        }
        bool foundCurrent = false;
        bool foundOther = false;
        ::jxx::lang::jshort relativePosition =
            Node::DOCUMENT_POSITION_DISCONNECTED;

        std::function<void(pugi::xml_node)> visitInDocumentOrder =
            [&](pugi::xml_node current) {
                if (foundOther) {
                    return;
                }

                for (auto currentChild = current.first_child();
                     currentChild;
                     currentChild = currentChild.next_sibling()) {

                    if (currentChild == node_) {
                        foundCurrent = true;
                    }

                    if (currentChild == value->node_) {
                        relativePosition =
                            foundCurrent
                                ? Node::DOCUMENT_POSITION_FOLLOWING
                                : Node::DOCUMENT_POSITION_PRECEDING;
                        foundOther = true;
                        return;
                    }

                    visitInDocumentOrder(currentChild);

                    if (foundOther) {
                        return;
                    }
                }
            };

        if (store_->document == node_) {
            foundCurrent = true;
        }

        if (store_->document == value->node_) {
            return foundCurrent
                ? Node::DOCUMENT_POSITION_FOLLOWING
                : Node::DOCUMENT_POSITION_PRECEDING;
        }

        visitInDocumentOrder(store_->document);
        return relativePosition;
    }

    ::jxx::lang::jbool isSupported(
        const ::jxx::Ptr<String>& feature,
        const ::jxx::Ptr<String>& version) const override {
        return implementation()->hasFeature(feature, version);
    }

    ::jxx::lang::jbool isEqualNode(
        const ::jxx::Ptr<Node>& other) const override {
        const auto value = ::jxx::CAST<DomNode>(other);
        if (!value || getNodeType() != value->getNodeType()) return false;
        return getNodeName()->equals(value->getNodeName()) &&
            getNodeValue()->equals(value->getNodeValue()) &&
            getTextContent()->equals(value->getTextContent());
    }

    ::jxx::Ptr<String> lookupPrefix(
        const ::jxx::Ptr<String>& namespaceURI) const override {
        if (!namespaceURI) return nullptr;
        const std::string wanted = namespaceURI->utf8();
        for (auto current = node_; current; current = current.parent()) {
            for (const auto& attribute : current.attributes()) {
                const std::string name = attribute.name();
                if (name.rfind("xmlns:", 0) == 0 && attribute.value() == wanted) {
                    return ::jxx::NEW<String>(name.substr(6));
                }
            }
        }
        return nullptr;
    }

    ::jxx::lang::jbool isDefaultNamespace(
        const ::jxx::Ptr<String>& namespaceURI) const override {
        const auto resolved = lookupNamespaceURI(nullptr);
        if (!resolved) return namespaceURI == nullptr || namespaceURI->utf8().empty();
        return namespaceURI != nullptr && resolved->equals(namespaceURI);
    }

    ::jxx::Ptr<String> lookupNamespaceURI(
        const ::jxx::Ptr<String>& prefix) const override {
        const std::string wanted = prefix ? prefix->utf8() : std::string();
        const std::string declaration = wanted.empty() ? "xmlns" : "xmlns:" + wanted;
        for (auto current = node_; current; current = current.parent()) {
            const auto attribute = current.attribute(declaration.c_str());
            if (attribute) return ::jxx::NEW<String>(attribute.value());
        }
        return nullptr;
    }

    ::jxx::lang::jbool isSameNode(
        const ::jxx::Ptr<Node>& other) const override {
        const auto value = ::jxx::CAST<DomNode>(other);
        return static_cast<::jxx::lang::jbool>(
            value && value->store_ == store_ && value->node_ == node_);
    }

    ::jxx::Ptr<::jxx::org::w3c::dom::DocumentType> getDoctype() const override {
        return nullptr;
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

    ::jxx::Ptr<Comment> createComment(
        const ::jxx::Ptr<String>& data) override {
        auto value = store_->document.append_child(pugi::node_comment);
        value.set_value(data ? data->utf8().c_str() : "");
        return wrapComment(store_, value);
    }

    ::jxx::Ptr<CDATASection> createCDATASection(
        const ::jxx::Ptr<String>& data) override {
        auto value = store_->document.append_child(pugi::node_cdata);
        value.set_value(data ? data->utf8().c_str() : "");
        return wrapCDATASection(store_, value);
    }

    ::jxx::Ptr<ProcessingInstruction>
    createProcessingInstruction(
        const ::jxx::Ptr<String>& target,
        const ::jxx::Ptr<String>& data) override {
        if (target == nullptr || target->utf8().empty()) {
            throw DOMException(
                DOMException::INVALID_CHARACTER_ERR,
                ::jxx::NEW<String>("Processing instruction target is empty"));
        }
        auto value = store_->document.append_child(pugi::node_pi);
        value.set_name(target->utf8().c_str());
        value.set_value(data ? data->utf8().c_str() : "");
        return wrapProcessingInstruction(store_, value);
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


    ::jxx::Ptr<Element> createElementNS(
        const ::jxx::Ptr<String>& namespaceURI,
        const ::jxx::Ptr<String>& qualifiedName) override {

        if (qualifiedName == nullptr || qualifiedName->utf8().empty()) {
            throw DOMException(
                DOMException::INVALID_CHARACTER_ERR,
                ::jxx::NEW<String>("qualifiedName is null or empty"));
        }

        const std::string qualifiedNameValue = qualifiedName->utf8();
        const std::string namespaceValue =
            namespaceURI == nullptr
                ? std::string()
                : namespaceURI->utf8();
        const auto separator = qualifiedNameValue.find(':');

        if (separator == 0 ||
            separator == qualifiedNameValue.size() - 1 ||
            (separator != std::string::npos &&
             qualifiedNameValue.find(':', separator + 1) != std::string::npos)) {
            throw DOMException(
                DOMException::NAMESPACE_ERR,
                ::jxx::NEW<String>("Invalid qualified name"));
        }

        if (separator != std::string::npos && namespaceValue.empty()) {
            throw DOMException(
                DOMException::NAMESPACE_ERR,
                ::jxx::NEW<String>(
                    "A prefixed element requires a namespace URI"));
        }

        auto element = store_->document.append_child(
            qualifiedNameValue.c_str());

        if (!namespaceValue.empty()) {
            const std::string prefix = prefixPart(qualifiedNameValue);
            const std::string declarationName =
                prefix.empty()
                    ? std::string("xmlns")
                    : std::string("xmlns:") + prefix;

            auto declaration =
                element.append_attribute(declarationName.c_str());
            declaration.set_value(namespaceValue.c_str());
        }

        return ::jxx::CAST<Element>(wrap(store_, element));
    }

    ::jxx::Ptr<Attr> createAttributeNS(
        const ::jxx::Ptr<String>&,
        const ::jxx::Ptr<String>& qualifiedName) override {
        return createAttribute(qualifiedName);
    }

    ::jxx::Ptr<Element> getElementById(
        const ::jxx::Ptr<String>& elementId) const override {
        if (elementId == nullptr) return nullptr;
        std::function<pugi::xml_node(pugi::xml_node)> scan = [&](pugi::xml_node current) -> pugi::xml_node {
            if (current.type() == pugi::node_element) {
                for (const auto& name : store_->idAttributes) {
                    auto attribute = current.attribute(name.c_str());
                    if (attribute && attribute.value() == elementId->utf8()) return current;
                }
            }
            for (const auto& child : current.children()) {
                auto match = scan(child);
                if (match) return match;
            }
            return {};
        };
        auto match = scan(store_->document);
        return match ? ::jxx::CAST<Element>(wrap(store_, match)) : nullptr;
    }

    ::jxx::Ptr<Node> importNode(
        const ::jxx::Ptr<Node>& importedNode,
        ::jxx::lang::jbool deep) override {
        const auto source = ::jxx::CAST<DomNode>(importedNode);
        if (source == nullptr || source->attributeNode_) {
            throw DOMException(
                DOMException::NOT_SUPPORTED_ERR,
                ::jxx::NEW<String>("Unsupported node type for import"));
        }

        auto imported = store_->document.append_copy(source->node_);
        if (!deep) {
            while (imported.first_child()) {
                imported.remove_child(imported.first_child());
            }
        }

        const auto destination = wrap(store_, imported);
        notifyUserDataHandlers(
            source->store_,
            userDataNodeKey(
                source->node_,
                source->attribute_,
                source->attributeNode_),
            UserDataHandler::NODE_IMPORTED,
            importedNode,
            destination);
        return destination;
    }

    ::jxx::Ptr<Node> adoptNode(
        const ::jxx::Ptr<Node>& sourceNode) override {
        const auto source = ::jxx::CAST<DomNode>(sourceNode);
        if (source == nullptr || source->attributeNode_) {
            throw DOMException(
                DOMException::NOT_SUPPORTED_ERR,
                ::jxx::NEW<String>("Unsupported node type for adoption"));
        }

        const auto sourceKey = userDataNodeKey(
            source->node_,
            source->attribute_,
            source->attributeNode_);

        if (source->store_ == store_) {
            notifyUserDataHandlers(
                source->store_,
                sourceKey,
                UserDataHandler::NODE_ADOPTED,
                sourceNode,
                nullptr);
            return sourceNode;
        }

        auto adopted = store_->document.append_copy(source->node_);
        const auto destination = wrap(store_, adopted);

        const auto values = source->store_->userData.find(sourceKey);
        if (values != source->store_->userData.end()) {
            store_->userData[userDataNodeKey(adopted, {}, false)] =
                values->second;
        }

        notifyUserDataHandlers(
            source->store_,
            sourceKey,
            UserDataHandler::NODE_ADOPTED,
            sourceNode,
            destination);

        auto parent = source->node_.parent();
        if (parent) {
            parent.remove_child(source->node_);
        }
        source->store_->userData.erase(sourceKey);
        return destination;
    }

    ::jxx::Ptr<Node> renameNode(
        const ::jxx::Ptr<Node>& sourceNode,
        const ::jxx::Ptr<String>& namespaceURI,
        const ::jxx::Ptr<String>& qualifiedName) override {
        const auto source = ::jxx::CAST<DomNode>(sourceNode);
        if (source == nullptr || source->store_ != store_) {
            throw DOMException(
                DOMException::WRONG_DOCUMENT_ERR,
                ::jxx::NEW<String>("Node belongs to another document"));
        }
        if (qualifiedName == nullptr || qualifiedName->utf8().empty()) {
            throw DOMException(
                DOMException::INVALID_CHARACTER_ERR,
                ::jxx::NEW<String>("qualifiedName is null or empty"));
        }

        const std::string name = qualifiedName->utf8();
        const std::string uri = namespaceURI ? namespaceURI->utf8() : std::string();
        const auto separator = name.find(':');
        if (separator != std::string::npos && uri.empty()) {
            throw DOMException(
                DOMException::NAMESPACE_ERR,
                ::jxx::NEW<String>("A prefixed name requires a namespace URI"));
        }

        if (source->attributeNode_) {
            source->attribute_.set_name(name.c_str());
        }
        else {
            source->node_.set_name(name.c_str());
        }

        if (!uri.empty()) {
            const std::string prefix = prefixPart(name);
            const std::string declarationName = prefix.empty()
                ? std::string("xmlns")
                : std::string("xmlns:") + prefix;
            pugi::xml_node declarationOwner = source->attributeNode_
                ? source->node_
                : source->node_;
            auto declaration = declarationOwner.attribute(declarationName.c_str());
            if (!declaration) {
                declaration = declarationOwner.append_attribute(declarationName.c_str());
            }
            declaration.set_value(uri.c_str());
        }
        notifyUserDataHandlers(
            source->store_,
            userDataNodeKey(
                source->node_,
                source->attribute_,
                source->attributeNode_),
            UserDataHandler::NODE_RENAMED,
            sourceNode,
            sourceNode);
        return sourceNode;
    }

    ::jxx::Ptr<String> getInputEncoding() const override { return store_->inputEncoding; }
    ::jxx::Ptr<String> getXmlEncoding() const override { return store_->xmlEncoding; }
    ::jxx::lang::jbool getXmlStandalone() const override { return store_->xmlStandalone; }
    void setXmlStandalone(::jxx::lang::jbool value) override { store_->xmlStandalone = value; }
    ::jxx::Ptr<String> getXmlVersion() const override { return store_->xmlVersion; }

    void setXmlVersion(const ::jxx::Ptr<String>& version) override {
        if (version == nullptr || (version->utf8() != "1.0" && version->utf8() != "1.1")) {
            throw DOMException(DOMException::NOT_SUPPORTED_ERR, ::jxx::NEW<String>("Unsupported XML version"));
        }
        store_->xmlVersion = version;
    }

    ::jxx::lang::jbool getStrictErrorChecking() const override { return store_->strictErrorChecking; }
    void setStrictErrorChecking(::jxx::lang::jbool value) override { store_->strictErrorChecking = value; }
    ::jxx::Ptr<String> getDocumentURI() const override { return store_->documentURI; }
    void setDocumentURI(const ::jxx::Ptr<String>& uri) override { store_->documentURI = uri; }
    ::jxx::Ptr<DOMConfiguration> getDomConfig() const override {
        return ::jxx::NEW<DOMConfigurationImpl>(store_);
    }

    void normalizeDocument() override {
        const auto comments =
            store_->domBooleanParameters["comments"];
        const auto cdataSections =
            store_->domBooleanParameters["cdata-sections"];

        std::function<void(pugi::xml_node)> normalizeChildren;
        normalizeChildren = [&](pugi::xml_node parent) {
            for (auto child = parent.first_child(); child;) {
                auto next = child.next_sibling();

                if (child.type() == pugi::node_comment && !comments) {
                    parent.remove_child(child);
                    child = next;
                    continue;
                }

                if (child.type() == pugi::node_element) {
                    normalizeChildren(child);
                }

                if (child.type() == pugi::node_cdata && !cdataSections) {
                    auto text = parent.insert_child_before(
                        pugi::node_pcdata,
                        child);
                    text.set_value(child.value());
                    parent.remove_child(child);
                    child = text;
                    next = child.next_sibling();
                }

                if (child.type() == pugi::node_pcdata) {
                    std::string combined = child.value();
                    auto adjacent = child.next_sibling();

                    while (adjacent &&
                           (adjacent.type() == pugi::node_pcdata ||
                            (!cdataSections &&
                             adjacent.type() == pugi::node_cdata))) {
                        combined += adjacent.value();
                        auto afterAdjacent = adjacent.next_sibling();
                        parent.remove_child(adjacent);
                        adjacent = afterAdjacent;
                    }

                    if (combined.empty()) {
                        parent.remove_child(child);
                    }
                    else {
                        child.set_value(combined.c_str());
                    }
                    child = adjacent;
                    continue;
                }

                child = next;
            }
        };

        normalizeChildren(store_->document);
    }

    ::jxx::Ptr<::jxx::lang::Object> setUserData(
        const ::jxx::Ptr<String>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& data,
        const ::jxx::Ptr<UserDataHandler>& handler) override {
        if (key == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        const auto nodeKey = userDataNodeKey(
            node_, attribute_, attributeNode_);
        auto& values = store_->userData[nodeKey];
        const auto found = values.find(key->utf8());
        const auto previous = found == values.end()
            ? nullptr
            : found->second.data;

        if (data == nullptr) {
            if (found != values.end()) {
                values.erase(found);
            }
            if (values.empty()) {
                store_->userData.erase(nodeKey);
            }
        }
        else {
            values[key->utf8()] = UserDataEntry{data, handler};
        }

        return previous;
    }

    ::jxx::Ptr<::jxx::lang::Object> getUserData(
        const ::jxx::Ptr<String>& key) const override {
        if (key == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        const auto values = store_->userData.find(
            userDataNodeKey(node_, attribute_, attributeNode_));
        if (values == store_->userData.end()) {
            return nullptr;
        }

        const auto found = values->second.find(key->utf8());
        return found == values->second.end()
            ? nullptr
            : found->second.data;
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


    ::jxx::Ptr<Attr> getAttributeNode(const ::jxx::Ptr<String>& name) const override {
        if (name == nullptr) return nullptr;
        auto attribute = node_.attribute(name->utf8().c_str());
        return attribute ? ::jxx::CAST<Attr>(::jxx::NEW<DomNode>(store_, node_, attribute)) : nullptr;
    }

    ::jxx::Ptr<Attr> setAttributeNode(const ::jxx::Ptr<Attr>& attribute) override {
        auto previous = getAttributeNode(attribute->getName());
        setAttribute(attribute->getName(), attribute->getValue());
        return previous;
    }

    ::jxx::Ptr<Attr> removeAttributeNode(const ::jxx::Ptr<Attr>& attribute) override {
        if (attribute == nullptr || !node_.remove_attribute(attribute->getName()->utf8().c_str())) {
            throw DOMException(DOMException::NOT_FOUND_ERR, ::jxx::NEW<String>("Attribute was not found"));
        }
        return attribute;
    }

    ::jxx::Ptr<String> getAttributeNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) const override {
        auto attribute = findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string());
        return ::jxx::NEW<String>(attribute ? attribute.value() : "");
    }

    void setAttributeNS(
        const ::jxx::Ptr<String>& namespaceURI,
        const ::jxx::Ptr<String>& qualifiedName,
        const ::jxx::Ptr<String>& value) override {

        if (qualifiedName == nullptr || qualifiedName->utf8().empty()) {
            throw DOMException(
                DOMException::INVALID_CHARACTER_ERR,
                ::jxx::NEW<String>("qualifiedName is null or empty"));
        }

        const std::string qualifiedNameValue = qualifiedName->utf8();
        const std::string namespaceValue =
            namespaceURI == nullptr
                ? std::string()
                : namespaceURI->utf8();
        const auto separator = qualifiedNameValue.find(':');

        if (separator != std::string::npos) {
            if (separator == 0 ||
                separator == qualifiedNameValue.size() - 1 ||
                qualifiedNameValue.find(':', separator + 1) != std::string::npos ||
                namespaceValue.empty()) {
                throw DOMException(
                    DOMException::NAMESPACE_ERR,
                    ::jxx::NEW<String>(
                        "Invalid namespaced attribute name"));
            }

            const std::string declarationName =
                std::string("xmlns:") + prefixPart(qualifiedNameValue);
            auto declaration = node_.attribute(declarationName.c_str());
            if (!declaration) {
                declaration = node_.append_attribute(declarationName.c_str());
            }
            declaration.set_value(namespaceValue.c_str());
        }

        setAttribute(qualifiedName, value);
    }

    void removeAttributeNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) override {
        auto attribute = findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string());
        if (attribute) node_.remove_attribute(attribute);
    }

    ::jxx::Ptr<Attr> getAttributeNodeNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) const override {
        auto attribute = findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string());
        return attribute ? ::jxx::CAST<Attr>(::jxx::NEW<DomNode>(store_, node_, attribute)) : nullptr;
    }

    ::jxx::Ptr<Attr> setAttributeNodeNS(const ::jxx::Ptr<Attr>& attribute) override { return setAttributeNode(attribute); }

    ::jxx::Ptr<NodeList> getElementsByTagNameNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) const override {
        std::vector<pugi::xml_node> nodes;
        const auto wantedUri = uri ? uri->utf8() : std::string();
        const auto wantedLocal = local ? local->utf8() : std::string();
        std::function<void(pugi::xml_node)> scan = [&](pugi::xml_node current) {
            for (const auto& child : current.children()) {
                const std::string name = child.name();
                if (child.type() == pugi::node_element &&
                    (wantedLocal == "*" || localPart(name) == wantedLocal) &&
                    (wantedUri == "*" || resolveNamespace(child, name) == wantedUri)) nodes.push_back(child);
                scan(child);
            }
        };
        scan(node_);
        return ::jxx::NEW<NodeListImpl>(store_, std::move(nodes));
    }

    ::jxx::lang::jbool hasAttributeNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) const override {
        return static_cast<::jxx::lang::jbool>(static_cast<bool>(findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string())));
    }

    ::jxx::Ptr<::jxx::org::w3c::dom::TypeInfo> getSchemaTypeInfo() const override { return nullptr; }

    void setIdAttribute(const ::jxx::Ptr<String>& name, ::jxx::lang::jbool isIdValue) override {
        if (!hasAttribute(name)) throw DOMException(DOMException::NOT_FOUND_ERR, ::jxx::NEW<String>("Attribute was not found"));
        if (isIdValue) store_->idAttributes.insert(name->utf8()); else store_->idAttributes.erase(name->utf8());
    }

    void setIdAttributeNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local, ::jxx::lang::jbool isIdValue) override {
        auto attribute = getAttributeNodeNS(uri, local);
        if (!attribute) throw DOMException(DOMException::NOT_FOUND_ERR, ::jxx::NEW<String>("Attribute was not found"));
        setIdAttribute(attribute->getName(), isIdValue);
    }

    void setIdAttributeNode(const ::jxx::Ptr<Attr>& attribute, ::jxx::lang::jbool isIdValue) override {
        if (!attribute) throw DOMException(DOMException::NOT_FOUND_ERR, ::jxx::NEW<String>("Attribute was not found"));
        setIdAttribute(attribute->getName(), isIdValue);
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

    ::jxx::lang::jbool getSpecified() const override { return true; }

    ::jxx::Ptr<Element> getOwnerElement() const override {
        return attributeNode_ ? ::jxx::CAST<Element>(wrap(store_, node_)) : nullptr;
    }

    ::jxx::lang::jbool isId() const override {
        return attributeNode_ && store_->idAttributes.count(attribute_.name()) != 0;
    }

    ::jxx::Ptr<String> getData() const override {
        return getNodeValue();
    }

    void setData(
        const ::jxx::Ptr<String>& value) override {
        setNodeValue(value);
    }

    ::jxx::lang::jint getLength() const override {
        return ::jxx::NEW<String>(
            node_.value())->length();
    }

    ::jxx::Ptr<String> substringData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count) const override {
        const auto value =
            ::jxx::NEW<String>(node_.value());

        const auto length = value->length();

        if (offset < 0 ||
            count < 0 ||
            offset > length) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>(
                    "Invalid character-data range"));
        }

        const auto actualCount =
            std::min(count, length - offset);

        return value->substring(
            offset,
            offset + actualCount);
    }

    void appendData(
        const ::jxx::Ptr<String>& data) override {
        auto value =
            utf16Value(
                ::jxx::NEW<String>(node_.value()));

        value.append(
            utf16Value(data));

        setNativeCharacterData(
            node_,
            value);
    }

    void insertData(
        ::jxx::lang::jint offset,
        const ::jxx::Ptr<String>& data) override {
        auto value =
            utf16Value(
                ::jxx::NEW<String>(node_.value()));

        if (offset < 0 ||
            static_cast<std::size_t>(offset) >
                value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>(
                    "Invalid character-data offset"));
        }

        value.insert(
            static_cast<std::size_t>(offset),
            utf16Value(data));

        setNativeCharacterData(
            node_,
            value);
    }

    void deleteData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count) override {
        auto value =
            utf16Value(
                ::jxx::NEW<String>(node_.value()));

        if (offset < 0 ||
            count < 0 ||
            static_cast<std::size_t>(offset) >
                value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>(
                    "Invalid character-data range"));
        }

        value.erase(
            static_cast<std::size_t>(offset),
            static_cast<std::size_t>(count));

        setNativeCharacterData(
            node_,
            value);
    }

    void replaceData(
        ::jxx::lang::jint offset,
        ::jxx::lang::jint count,
        const ::jxx::Ptr<String>& data) override {
        auto value =
            utf16Value(
                ::jxx::NEW<String>(node_.value()));

        if (offset < 0 ||
            count < 0 ||
            static_cast<std::size_t>(offset) >
                value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>(
                    "Invalid character-data range"));
        }

        value.replace(
            static_cast<std::size_t>(offset),
            static_cast<std::size_t>(count),
            utf16Value(data));

        setNativeCharacterData(
            node_,
            value);
    }

    ::jxx::Ptr<Text> splitText(
        ::jxx::lang::jint offset) override {
        auto value =
            utf16Value(
                ::jxx::NEW<String>(node_.value()));

        if (offset < 0 ||
            static_cast<std::size_t>(offset) >
                value.size()) {
            throw DOMException(
                DOMException::INDEX_SIZE_ERR,
                ::jxx::NEW<String>(
                    "Invalid text offset"));
        }

        const auto position =
            static_cast<std::size_t>(offset);

        const std::u16string tail =
            value.substr(position);

        value.erase(position);

        setNativeCharacterData(
            node_,
            value);

        auto next =
            node_.parent().insert_child_after(
                pugi::node_pcdata,
                node_);

        setNativeCharacterData(
            next,
            tail);

        return ::jxx::CAST<Text>(
            wrap(store_, next));
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

private:
    friend class NamedNodeMapImpl;

protected:
    std::shared_ptr<Store> store_;
    pugi::xml_node node_;
    pugi::xml_attribute attribute_;
    bool attributeNode_ = false;
};

class CommentNode final
    : public DomNode
    , public virtual Comment {
public:
    CommentNode(
        const std::shared_ptr<Store>& store,
        pugi::xml_node node)
        : DomNode(store, node) {
    }
};

class CDATASectionNode final
    : public DomNode
    , public virtual CDATASection {
public:
    CDATASectionNode(
        const std::shared_ptr<Store>& store,
        pugi::xml_node node)
        : DomNode(store, node) {
    }
};

class ProcessingInstructionNode final
    : public DomNode
    , public virtual ProcessingInstruction {
public:
    ProcessingInstructionNode(
        const std::shared_ptr<Store>& store,
        pugi::xml_node node)
        : DomNode(store, node) {
    }

    ::jxx::Ptr<String> getTarget() const override {
        return ::jxx::NEW<String>(node_.name());
    }

    ::jxx::Ptr<String> getData() const override {
        return ::jxx::NEW<String>(node_.value());
    }

    void setData(
        const ::jxx::Ptr<String>& data) override {
        node_.set_value(
            data != nullptr
                ? data->utf8().c_str()
                : "");
    }
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
        const ::jxx::Ptr<String>& name) const override {
        if (name == nullptr) {
            return nullptr;
        }
        const auto attribute = node_.attribute(name->utf8().c_str());
        if (!attribute) {
            return nullptr;
        }
        return ::jxx::CAST<Node>(
            ::jxx::NEW<DomNode>(store_, node_, attribute));
    }

    ::jxx::Ptr<Node> setNamedItem(
        const ::jxx::Ptr<Node>& value) override {
        const auto attributeNode = ::jxx::CAST<DomNode>(value);
        if (attributeNode == nullptr || !attributeNode->attributeNode_) {
            throw DOMException(
                DOMException::HIERARCHY_REQUEST_ERR,
                ::jxx::NEW<String>("Node is not an attribute"));
        }
        const auto name = attributeNode->attribute_.name();
        const auto previous = node_.attribute(name);
        ::jxx::Ptr<Node> result;
        if (previous) {
            result = ::jxx::CAST<Node>(
                ::jxx::NEW<DomNode>(store_, node_, previous));
        }
        auto destination = previous;
        if (!destination) {
            destination = node_.append_attribute(name);
        }
        destination.set_value(attributeNode->attribute_.value());
        return result;
    }

    ::jxx::Ptr<Node> removeNamedItem(
        const ::jxx::Ptr<String>& name) override {
        const auto attribute =
            name == nullptr ? pugi::xml_attribute()
                            : node_.attribute(name->utf8().c_str());
        if (!attribute) {
            throw DOMException(
                DOMException::NOT_FOUND_ERR,
                ::jxx::NEW<String>("Attribute was not found"));
        }
        const auto result = ::jxx::CAST<Node>(
            ::jxx::NEW<DomNode>(store_, node_, attribute));
        node_.remove_attribute(attribute);
        return result;
    }

    ::jxx::Ptr<Node> item(
        ::jxx::lang::jint index) const override {
        if (index < 0) {
            return nullptr;
        }
        ::jxx::lang::jint current = 0;
        for (const auto& attribute : node_.attributes()) {
            if (current == index) {
                return ::jxx::CAST<Node>(
                    ::jxx::NEW<DomNode>(store_, node_, attribute));
            }
            ++current;
        }
        return nullptr;
    }

    ::jxx::Ptr<Node> getNamedItemNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) const override {
        auto attribute = findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string());
        return attribute ? ::jxx::CAST<Node>(::jxx::NEW<DomNode>(store_, node_, attribute)) : nullptr;
    }

    ::jxx::Ptr<Node> setNamedItemNS(const ::jxx::Ptr<Node>& value) override { return setNamedItem(value); }

    ::jxx::Ptr<Node> removeNamedItemNS(const ::jxx::Ptr<String>& uri, const ::jxx::Ptr<String>& local) override {
        auto attribute = findAttributeNS(node_, uri ? uri->utf8() : std::string(), local ? local->utf8() : std::string());
        if (!attribute) throw DOMException(DOMException::NOT_FOUND_ERR, ::jxx::NEW<String>("Attribute was not found"));
        auto result = ::jxx::CAST<Node>(::jxx::NEW<DomNode>(store_, node_, attribute));
        node_.remove_attribute(attribute);
        return result;
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
            DOMException::HIERARCHY_REQUEST_ERR,
            ::jxx::NEW<String>("Unsupported node"));
    }

    if (value->store_ != store_) {
        throw DOMException(
            DOMException::WRONG_DOCUMENT_ERR,
            ::jxx::NEW<String>("Node belongs to another document"));
    }

    if (value->node_.parent() == node_) {
        return child;
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

::jxx::Ptr<Comment> wrapComment(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node) {
    return node
        ? ::jxx::CAST<Comment>(::jxx::NEW<CommentNode>(store, node))
        : nullptr;
}

::jxx::Ptr<CDATASection> wrapCDATASection(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node) {
    return node
        ? ::jxx::CAST<CDATASection>(
              ::jxx::NEW<CDATASectionNode>(store, node))
        : nullptr;
}

::jxx::Ptr<ProcessingInstruction> wrapProcessingInstruction(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node) {
    return node
        ? ::jxx::CAST<ProcessingInstruction>(
              ::jxx::NEW<ProcessingInstructionNode>(store, node))
        : nullptr;
}

::jxx::Ptr<Node> wrap(
    const std::shared_ptr<Store>& store,
    pugi::xml_node node) {
    if (!node) {
        return nullptr;
    }

    switch (node.type()) {
    case pugi::node_comment:
        return ::jxx::CAST<Node>(wrapComment(store, node));

    case pugi::node_cdata:
        return ::jxx::CAST<Node>(wrapCDATASection(store, node));

    case pugi::node_pi:
        return ::jxx::CAST<Node>(
            wrapProcessingInstruction(store, node));

    default:
        return ::jxx::CAST<Node>(
            ::jxx::NEW<DomNode>(store, node));
    }
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
