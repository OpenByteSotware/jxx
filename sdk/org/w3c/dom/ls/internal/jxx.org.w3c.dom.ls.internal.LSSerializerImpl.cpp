#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSSerializerImpl.h"

#include <string>

#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.Writer.h"

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializerFilter.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"

namespace jxx::org::w3c::dom::ls::internal {
namespace {

std::string textOf(const ::jxx::Ptr<::jxx::lang::String>& value) {
    return value == nullptr ? std::string() : value->utf8();
}

std::string escapeText(const std::string& input, bool attribute) {
    std::string result;
    for (const char value : input) {
        switch (value) {
        case '&': result += "&amp;"; break;
        case '<': result += "&lt;"; break;
        case '>': result += "&gt;"; break;
        case '"': result += attribute ? "&quot;" : "\""; break;
        default: result += value; break;
        }
    }
    return result;
}

void appendNode(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node,
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter>& filter,
    std::string& output) {
    using Node = ::jxx::org::w3c::dom::Node;
    using NodeFilter = ::jxx::org::w3c::dom::traversal::NodeFilter;

    if (node == nullptr) {
        return;
    }

    if (filter != nullptr) {
        const auto decision = filter->acceptNode(node);
        if (decision == NodeFilter::FILTER_REJECT) {
            return;
        }
        if (decision == NodeFilter::FILTER_SKIP) {
            const auto children = node->getChildNodes();
            if (children != nullptr) {
                for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
                    appendNode(children->item(index), filter, output);
                }
            }
            return;
        }
    }

    const auto type = node->getNodeType();
    if (type == Node::DOCUMENT_NODE) {
        const auto document =
            ::jxx::CAST<::jxx::org::w3c::dom::Document>(node);
        if (document != nullptr) {
            appendNode(
                ::jxx::CAST<::jxx::org::w3c::dom::Node>(
                    document->getDocumentElement()),
                filter,
                output);
        }
        return;
    }

    if (type == Node::DOCUMENT_FRAGMENT_NODE) {
        const auto children = node->getChildNodes();
        if (children != nullptr) {
            for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
                appendNode(children->item(index), filter, output);
            }
        }
        return;
    }

    if (type == Node::ELEMENT_NODE) {
        const std::string name = textOf(node->getNodeName());
        output += '<' + name;
        const auto attributes = node->getAttributes();
        if (attributes != nullptr) {
            for (::jxx::lang::jint index = 0; index < attributes->getLength(); ++index) {
                const auto attribute = attributes->item(index);
                if (attribute != nullptr) {
                    output += ' ' + textOf(attribute->getNodeName()) + "=\"";
                    output += escapeText(textOf(attribute->getNodeValue()), true) + '"';
                }
            }
        }
        const auto children = node->getChildNodes();
        if (children == nullptr || children->getLength() == 0) {
            output += "/>";
            return;
        }
        output += '>';
        for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
            appendNode(children->item(index), filter, output);
        }
        output += "</" + name + '>';
        return;
    }

    if (type == Node::TEXT_NODE) {
        output += escapeText(textOf(node->getNodeValue()), false);
    } else if (type == Node::CDATA_SECTION_NODE) {
        output += "<![CDATA[" + textOf(node->getNodeValue()) + "]]>";
    } else if (type == Node::COMMENT_NODE) {
        output += "<!--" + textOf(node->getNodeValue()) + "-->";
    } else if (type == Node::PROCESSING_INSTRUCTION_NODE) {
        output += "<?" + textOf(node->getNodeName());
        const auto value = textOf(node->getNodeValue());
        if (!value.empty()) output += ' ' + value;
        output += "?>";
    }
}

} // namespace

LSSerializerImpl::LSSerializerImpl()
    : Super() {
}

LSSerializerImpl::~LSSerializerImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>
LSSerializerImpl::getDomConfig() const { return nullptr; }

::jxx::Ptr<::jxx::lang::String> LSSerializerImpl::getNewLine() const { return newLine_; }
void LSSerializerImpl::setNewLine(const ::jxx::Ptr<::jxx::lang::String>& value) { newLine_ = value; }
::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter> LSSerializerImpl::getFilter() const { return filter_; }
void LSSerializerImpl::setFilter(const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter>& value) { filter_ = value; }

::jxx::lang::jbool LSSerializerImpl::write(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSOutput>& destination) {
    if (nodeArg == nullptr || destination == nullptr) {
        throw ::jxx::org::w3c::dom::ls::LSException(
            ::jxx::org::w3c::dom::ls::LSException::SERIALIZE_ERR,
            ::jxx::NEW<::jxx::lang::String>("Missing node or output destination"));
    }

    const auto serialized = writeToString(nodeArg);
    const auto characterStream = destination->getCharacterStream();
    if (characterStream != nullptr) {
        characterStream->write(serialized);
        characterStream->flush();
        return true;
    }

    const auto byteStream = destination->getByteStream();
    if (byteStream != nullptr) {
        const std::string bytes = serialized->utf8();
        for (const unsigned char value : bytes) {
            byteStream->write(static_cast<::jxx::lang::jint>(value));
        }
        byteStream->flush();
        return true;
    }

    const auto systemId = destination->getSystemId();
    if (systemId != nullptr && !systemId->utf8().empty()) {
        return writeToURI(nodeArg, systemId);
    }

    throw ::jxx::org::w3c::dom::ls::LSException(
        ::jxx::org::w3c::dom::ls::LSException::SERIALIZE_ERR,
        ::jxx::NEW<::jxx::lang::String>("No supported output destination"));
}

::jxx::lang::jbool LSSerializerImpl::writeToURI(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
    const ::jxx::Ptr<::jxx::lang::String>& uri) {
    if (nodeArg == nullptr || uri == nullptr || uri->utf8().empty()) {
        throw ::jxx::org::w3c::dom::ls::LSException(
            ::jxx::org::w3c::dom::ls::LSException::SERIALIZE_ERR,
            ::jxx::NEW<::jxx::lang::String>("Missing node or URI"));
    }

    std::string path = uri->utf8();
    const std::string filePrefix = "file://";
    if (path.compare(0, filePrefix.size(), filePrefix) == 0) {
        path.erase(0, filePrefix.size());
#ifdef _WIN32
        if (path.size() >= 3 && path.front() == '/' && path[2] == ':') {
            path.erase(path.begin());
        }
#endif
    } else if (path.find("://") != std::string::npos) {
        throw ::jxx::org::w3c::dom::ls::LSException(
            ::jxx::org::w3c::dom::ls::LSException::SERIALIZE_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unsupported URI scheme"));
    }

    try {
        const auto stream = ::jxx::NEW<::jxx::io::FileOutputStream>(
            ::jxx::NEW<::jxx::lang::String>(path.c_str()));
        const std::string bytes = writeToString(nodeArg)->utf8();
        for (const unsigned char value : bytes) {
            stream->write(static_cast<::jxx::lang::jint>(value));
        }
        stream->flush();
        stream->close();
        return true;
    } catch (...) {
        throw ::jxx::org::w3c::dom::ls::LSException(
            ::jxx::org::w3c::dom::ls::LSException::SERIALIZE_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unable to write URI"));
    }
}

::jxx::Ptr<::jxx::lang::String> LSSerializerImpl::writeToString(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg) {
    std::string output;
    appendNode(nodeArg, filter_, output);
    return ::jxx::NEW<::jxx::lang::String>(output.c_str());
}

} // namespace jxx::org::w3c::dom::ls::internal
