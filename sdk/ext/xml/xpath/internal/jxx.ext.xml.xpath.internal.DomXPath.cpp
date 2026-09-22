#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.DomXPath.h"

#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

#include <vector>

namespace jxx::ext::xml::xpath::internal {
namespace {
using Node = ::jxx::org::w3c::dom::Node;
using NodeList = ::jxx::org::w3c::dom::NodeList;

class SnapshotNodeList final
    : public ::jxx::lang::Object,
      public NodeList {
public:
    explicit SnapshotNodeList(std::vector<::jxx::Ptr<Node>> values)
        : values_(std::move(values)) {}
    ::jxx::Ptr<Node> item(::jxx::lang::jint index) const override {
        return index < 0 || static_cast<std::size_t>(index) >= values_.size()
            ? nullptr : values_[static_cast<std::size_t>(index)];
    }
    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }
private:
    std::vector<::jxx::Ptr<Node>> values_;
};

class CompiledExpression final
    : public ::jxx::lang::Object,
      public ::jxx::ext::xml::xpath::XPathExpression {
public:
    explicit CompiledExpression(const ::jxx::Ptr<::jxx::lang::String>& expression)
        : expression_(expression) {}
    ::jxx::Ptr<::jxx::lang::Object> evaluate(
        const ::jxx::Ptr<::jxx::lang::Object>& item,
        const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType) override;
    ::jxx::Ptr<::jxx::lang::String> evaluate(
        const ::jxx::Ptr<::jxx::lang::Object>& item) override;
private:
    ::jxx::Ptr<::jxx::lang::String> expression_;
};

::jxx::Ptr<Node> asNode(const ::jxx::Ptr<::jxx::lang::Object>& item) {
    const auto node = std::dynamic_pointer_cast<Node>(item);
    if (node == nullptr) {
        throw ::jxx::ext::xml::xpath::XPathExpressionException(
            ::jxx::NEW<::jxx::lang::String>("XPath context is not a DOM node"));
    }
    return node;
}

std::vector<std::string> split(const std::string& expression) {
    std::vector<std::string> result;
    std::size_t start = 0U;
    while (start < expression.size()) {
        while (start < expression.size() && expression[start] == '/') ++start;
        if (start >= expression.size()) break;
        const auto end = expression.find('/', start);
        result.push_back(expression.substr(start, end == std::string::npos ? std::string::npos : end - start));
        if (end == std::string::npos) break;
        start = end + 1U;
    }
    return result;
}

bool named(const ::jxx::Ptr<Node>& node, const std::string& name) {
    return name == "*" || (node != nullptr && node->getNodeName() != nullptr && node->getNodeName()->utf8() == name);
}

void descendants(const ::jxx::Ptr<Node>& node, const std::string& name, std::vector<::jxx::Ptr<Node>>& out) {
    const auto children = node->getChildNodes();
    if (children == nullptr) return;
    for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
        const auto child = children->item(index);
        if (child == nullptr) continue;
        if (child->getNodeType() == Node::ELEMENT_NODE && named(child, name)) out.push_back(child);
        descendants(child, name, out);
    }
}

std::vector<::jxx::Ptr<Node>> select(const ::jxx::Ptr<Node>& context, const std::string& expression) {
    if (expression == ".") return {context};
    const bool descendant = expression.rfind("//", 0U) == 0U;
    const bool absolute = !descendant && !expression.empty() && expression.front() == '/';
    const auto steps = split(expression);
    if (steps.empty()) throw ::jxx::ext::xml::xpath::XPathExpressionException(
        ::jxx::NEW<::jxx::lang::String>("Empty XPath expression"));
    if (descendant) {
        std::vector<::jxx::Ptr<Node>> found;
        descendants(context, steps.front(), found);
        return found;
    }
    ::jxx::Ptr<Node> root = context;
    if (absolute) {
        while (root->getParentNode() != nullptr) root = root->getParentNode();
        if (root->getNodeType() == Node::DOCUMENT_NODE) {
            root = std::dynamic_pointer_cast<::jxx::org::w3c::dom::Document>(root)->getDocumentElement();
        }
    }
    std::vector<::jxx::Ptr<Node>> current{root};
    std::size_t stepIndex = 0U;
    if (absolute && named(root, steps.front())) stepIndex = 1U;
    for (; stepIndex < steps.size(); ++stepIndex) {
        std::vector<::jxx::Ptr<Node>> next;
        for (const auto& node : current) {
            const auto children = node->getChildNodes();
            if (children == nullptr) continue;
            for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
                const auto child = children->item(index);
                if (child != nullptr && child->getNodeType() == Node::ELEMENT_NODE && named(child, steps[stepIndex])) next.push_back(child);
            }
        }
        current = std::move(next);
    }
    return current;
}

bool same(const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& left,
          const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& right) {
    return left != nullptr && right != nullptr && left->equals(::jxx::CAST<::jxx::lang::Object>(right));
}

::jxx::Ptr<::jxx::lang::Object> evaluateValue(
    const ::jxx::Ptr<::jxx::lang::String>& expression,
    const ::jxx::Ptr<::jxx::lang::Object>& item,
    const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType) {
    if (expression == nullptr || returnType == nullptr) throw ::jxx::lang::NullPointerException();
    const auto nodes = select(asNode(item), expression->utf8());
    if (same(returnType, ::jxx::ext::xml::xpath::XPathConstants::NODESET())) {
        return std::make_shared<SnapshotNodeList>(nodes);
    }
    const auto first = nodes.empty() ? nullptr : nodes.front();
    if (same(returnType, ::jxx::ext::xml::xpath::XPathConstants::NODE())) {
        return first == nullptr
            ? nullptr
            : std::dynamic_pointer_cast<::jxx::lang::Object>(first);
    }
    const auto text = first == nullptr || first->getTextContent() == nullptr ? "" : first->getTextContent()->utf8();
    if (same(returnType, ::jxx::ext::xml::xpath::XPathConstants::STRING())) return ::jxx::NEW<::jxx::lang::String>(text);
    if (same(returnType, ::jxx::ext::xml::xpath::XPathConstants::BOOLEAN())) return ::jxx::lang::Boolean::valueOf(!nodes.empty());
    if (same(returnType, ::jxx::ext::xml::xpath::XPathConstants::NUMBER())) {
        return ::jxx::lang::Double::valueOf(::jxx::lang::Double::parseDouble(::jxx::NEW<::jxx::lang::String>(text)));
    }
    throw ::jxx::ext::xml::xpath::XPathExpressionException(
        ::jxx::NEW<::jxx::lang::String>("Unsupported XPath return type"));
}

::jxx::Ptr<::jxx::lang::Object> CompiledExpression::evaluate(
    const ::jxx::Ptr<::jxx::lang::Object>& item,
    const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType) {
    return evaluateValue(expression_, item, returnType);
}
::jxx::Ptr<::jxx::lang::String> CompiledExpression::evaluate(
    const ::jxx::Ptr<::jxx::lang::Object>& item) {
    return ::jxx::CAST<::jxx::lang::String>(evaluateValue(expression_, item, ::jxx::ext::xml::xpath::XPathConstants::STRING()));
}
} // namespace

void DomXPath::reset() {}
::jxx::Ptr<::jxx::ext::xml::xpath::XPathExpression> DomXPath::compile(
    const ::jxx::Ptr<::jxx::lang::String>& expression) {
    if (expression == nullptr) throw ::jxx::lang::NullPointerException();
    return std::make_shared<CompiledExpression>(expression);
}
::jxx::Ptr<::jxx::lang::Object> DomXPath::evaluate(
    const ::jxx::Ptr<::jxx::lang::String>& expression,
    const ::jxx::Ptr<::jxx::lang::Object>& item,
    const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType) {
    return evaluateValue(expression, item, returnType);
}
::jxx::Ptr<::jxx::lang::String> DomXPath::evaluate(
    const ::jxx::Ptr<::jxx::lang::String>& expression,
    const ::jxx::Ptr<::jxx::lang::Object>& item) {
    return ::jxx::CAST<::jxx::lang::String>(evaluateValue(expression, item, ::jxx::ext::xml::xpath::XPathConstants::STRING()));
}
} // namespace jxx::ext::xml::xpath::internal
