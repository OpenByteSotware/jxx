#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSParserImpl.h"

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParserFilter.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMConfigurationImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace jxx::org::w3c::dom::ls::internal {
namespace {

::jxx::org::w3c::dom::ls::LSException parseFailure(
    const char* message) {
    return ::jxx::org::w3c::dom::ls::LSException(
        ::jxx::org::w3c::dom::ls::LSException::PARSE_ERR,
        ::jxx::NEW<::jxx::lang::String>(message));
}

bool configurationFlag(
    const ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>& configuration,
    const char* name,
    bool defaultValue) {
    if (configuration == nullptr) {
        return defaultValue;
    }
    const auto value = configuration->getParameter(
        ::jxx::NEW<::jxx::lang::String>(name));
    const auto booleanValue = ::jxx::CAST<::jxx::lang::Boolean>(value);
    return booleanValue == nullptr
        ? defaultValue
        : booleanValue->booleanValue();
}

::jxx::Ptr<::jxx::ext::xml::parsers::DocumentBuilder> createBuilder(
    const ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>& configuration) {
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    factory->setNamespaceAware(
        configurationFlag(configuration, "namespaces", true));
    factory->setIgnoringComments(
        !configurationFlag(configuration, "comments", true));
    factory->setValidating(
        configurationFlag(configuration, "validate", false) ||
        configurationFlag(configuration, "validate-if-schema", false));
    return factory->newDocumentBuilder();
}

::jxx::lang::jint visibilityMask(
    ::jxx::lang::jshort nodeType) {
    if (nodeType < 1 || nodeType > 32) {
        return 0;
    }
    return static_cast<::jxx::lang::jint>(1U << (nodeType - 1));
}

bool isVisible(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& filter,
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    if (filter == nullptr || node == nullptr) {
        return false;
    }
    const auto whatToShow = filter->getWhatToShow();
    return whatToShow ==
            ::jxx::org::w3c::dom::traversal::NodeFilter::SHOW_ALL ||
        (whatToShow & visibilityMask(node->getNodeType())) != 0;
}

void checkInterrupt(::jxx::lang::jshort decision) {
    if (decision ==
        ::jxx::org::w3c::dom::ls::LSParserFilter::FILTER_INTERRUPT) {
        throw parseFailure("Parsing interrupted by filter");
    }
}

void removeNode(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    if (node == nullptr) {
        return;
    }
    const auto parent = node->getParentNode();
    if (parent != nullptr) {
        parent->removeChild(node);
    }
}

void promoteChildrenAndRemove(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    if (node == nullptr) {
        return;
    }
    const auto parent = node->getParentNode();
    if (parent == nullptr) {
        return;
    }
    while (node->getFirstChild() != nullptr) {
        parent->insertBefore(node->getFirstChild(), node);
    }
    parent->removeChild(node);
}

void applyFilterToNode(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node,
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& filter) {
    using Filter = ::jxx::org::w3c::dom::ls::LSParserFilter;
    using Node = ::jxx::org::w3c::dom::Node;

    if (node == nullptr || filter == nullptr) {
        return;
    }

    if (node->getNodeType() == Node::ELEMENT_NODE && isVisible(filter, node)) {
        const auto decision = filter->startElement(
            ::jxx::CAST<::jxx::org::w3c::dom::Element>(node));
        checkInterrupt(decision);
        if (decision == Filter::FILTER_REJECT) {
            removeNode(node);
            return;
        }
        if (decision == Filter::FILTER_SKIP) {
            auto child = node->getFirstChild();
            while (child != nullptr) {
                const auto next = child->getNextSibling();
                applyFilterToNode(child, filter);
                child = next;
            }
            promoteChildrenAndRemove(node);
            return;
        }
    }

    auto child = node->getFirstChild();
    while (child != nullptr) {
        const auto next = child->getNextSibling();
        applyFilterToNode(child, filter);
        child = next;
    }

    if (!isVisible(filter, node)) {
        return;
    }

    const auto decision = filter->acceptNode(node);
    checkInterrupt(decision);
    if (decision == Filter::FILTER_REJECT) {
        removeNode(node);
    } else if (decision == Filter::FILTER_SKIP) {
        promoteChildrenAndRemove(node);
    }
}

void applyFilter(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Document>& document,
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& filter) {
    if (document == nullptr || filter == nullptr) {
        return;
    }
    auto child = document->getFirstChild();
    while (child != nullptr) {
        const auto next = child->getNextSibling();
        applyFilterToNode(child, filter);
        child = next;
    }
}

} // namespace

LSParserImpl::LSParserImpl()
    : Super()
    , domConfig_(::jxx::CAST<::jxx::org::w3c::dom::DOMConfiguration>(
          ::jxx::NEW<DOMConfigurationImpl>(std::vector<std::string>{
              "comments", "namespaces", "namespace-declarations",
              "validate", "validate-if-schema", "well-formed"})))
    , busy_(false)
    , abortRequested_(false) {
}

LSParserImpl::~LSParserImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>
LSParserImpl::getDomConfig() const {
    return domConfig_;
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>
LSParserImpl::getFilter() const {
    return filter_;
}

void LSParserImpl::setFilter(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& value) {
    filter_ = value;
}

::jxx::lang::jbool LSParserImpl::getAsync() const {
    return false;
}

::jxx::lang::jbool LSParserImpl::getBusy() const {
    return busy_;
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> LSParserImpl::parse(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input) {
    if (input == nullptr) {
        throw parseFailure("No input specified");
    }
    if (busy_) {
        throw parseFailure("Parser is busy");
    }

    busy_ = true;
    abortRequested_ = false;
    try {
        const auto source = ::jxx::NEW<::jxx::org::xml::sax::InputSource>();
        const auto characterStream = input->getCharacterStream();
        const auto byteStream = input->getByteStream();
        const auto stringData = input->getStringData();

        if (characterStream != nullptr) {
            source->setCharacterStream(characterStream);
        } else if (byteStream != nullptr) {
            source->setByteStream(byteStream);
        } else if (stringData != nullptr && !stringData->utf8().empty()) {
            source->setCharacterStream(
                ::jxx::NEW<::jxx::io::StringReader>(stringData));
        } else if (input->getSystemId() != nullptr) {
            source->setSystemId(input->getSystemId());
        } else if (input->getPublicId() != nullptr) {
            source->setPublicId(input->getPublicId());
        } else {
            throw parseFailure("No input specified");
        }

        source->setEncoding(input->getEncoding());
        source->setPublicId(input->getPublicId());
        source->setSystemId(input->getSystemId());

        const auto builder = createBuilder(domConfig_);
        const auto document = builder->parse(source);
        if (abortRequested_) {
            throw parseFailure("Parsing aborted");
        }
        applyFilter(document, filter_);
        busy_ = false;
        return document;
    } catch (const ::jxx::org::w3c::dom::ls::LSException&) {
        busy_ = false;
        throw;
    } catch (...) {
        busy_ = false;
        throw parseFailure("Unable to parse input");
    }
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> LSParserImpl::parseURI(
    const ::jxx::Ptr<::jxx::lang::String>& uri) {
    if (uri == nullptr || uri->utf8().empty()) {
        throw parseFailure("No URI specified");
    }
    const auto input =
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(uri);
    try {
        busy_ = true;
        const auto document = createBuilder(domConfig_)->parse(input);
        busy_ = false;
        return document;
    } catch (...) {
        busy_ = false;
        throw parseFailure("Unable to parse URI");
    }
}

::jxx::Ptr<::jxx::org::w3c::dom::Node> LSParserImpl::parseWithContext(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input,
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& contextArg,
    ::jxx::lang::jshort action) {
    using Node = ::jxx::org::w3c::dom::Node;
    using Parser = ::jxx::org::w3c::dom::ls::LSParser;

    if (input == nullptr || contextArg == nullptr) {
        throw parseFailure("Missing input or context node");
    }
    if (busy_) {
        throw parseFailure("Parser is busy");
    }
    if (action < Parser::ACTION_APPEND_AS_CHILDREN ||
        action > Parser::ACTION_REPLACE) {
        throw parseFailure("Invalid context action");
    }

    const auto stringData = input->getStringData();
    if (stringData == nullptr) {
        throw parseFailure("Context parsing requires string data");
    }

    const auto targetDocument =
        contextArg->getNodeType() == Node::DOCUMENT_NODE
            ? ::jxx::CAST<::jxx::org::w3c::dom::Document>(contextArg)
            : contextArg->getOwnerDocument();
    if (targetDocument == nullptr) {
        throw parseFailure("Context node has no owner document");
    }

    const std::string wrapped =
        "<jxx_fragment_root>" + stringData->utf8() +
        "</jxx_fragment_root>";
    const auto wrappedInput =
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(
            ::jxx::NEW<::jxx::io::StringReader>(
                ::jxx::NEW<::jxx::lang::String>(wrapped.c_str())));

    ::jxx::Ptr<::jxx::org::w3c::dom::Document> parsed;
    try {
        busy_ = true;
        parsed = createBuilder(domConfig_)->parse(wrappedInput);
        busy_ = false;
    } catch (...) {
        busy_ = false;
        throw parseFailure("Unable to parse context fragment");
    }

    const auto wrapper = parsed->getDocumentElement();
    if (wrapper == nullptr || wrapper->getFirstChild() == nullptr) {
        return nullptr;
    }

    ::jxx::Ptr<Node> firstResult;
    ::jxx::Ptr<Node> reference;
    ::jxx::Ptr<Node> parent;

    if (action == Parser::ACTION_APPEND_AS_CHILDREN ||
        action == Parser::ACTION_REPLACE_CHILDREN) {
        parent = contextArg;
        if (action == Parser::ACTION_REPLACE_CHILDREN) {
            while (contextArg->getFirstChild() != nullptr) {
                contextArg->removeChild(contextArg->getFirstChild());
            }
        }
    } else {
        parent = contextArg->getParentNode();
        if (parent == nullptr) {
            throw parseFailure("Context node has no parent");
        }
        if (action == Parser::ACTION_INSERT_BEFORE ||
            action == Parser::ACTION_REPLACE) {
            reference = contextArg;
        } else {
            reference = contextArg->getNextSibling();
        }
    }

    auto sourceChild = wrapper->getFirstChild();
    ::jxx::Ptr<Node> previousInserted;
    while (sourceChild != nullptr) {
        const auto next = sourceChild->getNextSibling();
        const auto imported = targetDocument->importNode(sourceChild, true);
        if (firstResult == nullptr) {
            firstResult = imported;
        }

        if (action == Parser::ACTION_REPLACE && previousInserted == nullptr) {
            parent->replaceChild(imported, contextArg);
        } else if (action == Parser::ACTION_REPLACE) {
            const auto afterPrevious = previousInserted->getNextSibling();
            if (afterPrevious != nullptr) {
                parent->insertBefore(imported, afterPrevious);
            } else {
                parent->appendChild(imported);
            }
        } else if (reference != nullptr) {
            parent->insertBefore(imported, reference);
        } else {
            parent->appendChild(imported);
        }

        previousInserted = imported;
        sourceChild = next;
    }

    if (filter_ != nullptr) {
        auto current = firstResult;
        while (current != nullptr) {
            const auto next = current->getNextSibling();
            applyFilterToNode(current, filter_);
            current = next;
        }
    }

    return firstResult;
}

void LSParserImpl::abort() {
    abortRequested_ = true;
}

} // namespace jxx::org::w3c::dom::ls::internal
