#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformer.h"

#include <string>

#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerException.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

namespace jxx::ext::xml::transform::internal {
namespace {

std::string escapeText(const std::string& text, bool attribute) {
    std::string output;
    output.reserve(text.size());
    for (const char value : text) {
        switch (value) {
            case '&': output += "&amp;"; break;
            case '<': output += "&lt;"; break;
            case '>': output += "&gt;"; break;
            case '"': output += attribute ? "&quot;" : "\""; break;
            default: output.push_back(value); break;
        }
    }
    return output;
}

std::string stringValue(const ::jxx::Ptr<::jxx::lang::String>& value) {
    return value == nullptr ? std::string{} : value->utf8();
}

void serializeNode(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node,
    std::string& output,
    bool indent,
    ::jxx::lang::jint depth) {
    if (node == nullptr) {
        return;
    }

    const auto type = node->getNodeType();
    const auto indentation = [&]() {
        if (indent) output.append(static_cast<std::size_t>(depth * 2), ' ');
    };

    if (type == ::jxx::org::w3c::dom::Node::DOCUMENT_NODE ||
        type == ::jxx::org::w3c::dom::Node::DOCUMENT_FRAGMENT_NODE) {
        const auto children = node->getChildNodes();
        if (children != nullptr) {
            for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
                serializeNode(children->item(index), output, indent, depth);
            }
        }
        return;
    }

    if (type == ::jxx::org::w3c::dom::Node::ELEMENT_NODE) {
        indentation();
        const std::string name = stringValue(node->getNodeName());
        output += '<';
        output += name;

        const auto attributes = node->getAttributes();
        if (attributes != nullptr) {
            for (::jxx::lang::jint index = 0; index < attributes->getLength(); ++index) {
                const auto attribute = attributes->item(index);
                if (attribute == nullptr) continue;
                output += ' ';
                output += stringValue(attribute->getNodeName());
                output += "=\"";
                output += escapeText(stringValue(attribute->getNodeValue()), true);
                output += '"';
            }
        }

        const auto children = node->getChildNodes();
        if (children == nullptr || children->getLength() == 0) {
            output += "/>";
            if (indent) output += '\n';
            return;
        }

        output += '>';
        bool childElements = false;
        for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
            const auto child = children->item(index);
            if (child != nullptr &&
                (child->getNodeType() == ::jxx::org::w3c::dom::Node::ELEMENT_NODE ||
                 child->getNodeType() == ::jxx::org::w3c::dom::Node::COMMENT_NODE)) {
                childElements = true;
                break;
            }
        }
        if (indent && childElements) output += '\n';

        for (::jxx::lang::jint index = 0; index < children->getLength(); ++index) {
            serializeNode(children->item(index), output, indent, depth + 1);
        }

        if (indent && childElements) indentation();
        output += "</";
        output += name;
        output += '>';
        if (indent) output += '\n';
        return;
    }

    if (type == ::jxx::org::w3c::dom::Node::TEXT_NODE) {
        output += escapeText(stringValue(node->getNodeValue()), false);
        return;
    }

    if (type == ::jxx::org::w3c::dom::Node::CDATA_SECTION_NODE) {
        output += "<![CDATA[";
        output += stringValue(node->getNodeValue());
        output += "]]>";
        return;
    }

    if (type == ::jxx::org::w3c::dom::Node::COMMENT_NODE) {
        indentation();
        output += "<!--";
        output += stringValue(node->getNodeValue());
        output += "-->";
        if (indent) output += '\n';
        return;
    }

    if (type == ::jxx::org::w3c::dom::Node::PROCESSING_INSTRUCTION_NODE) {
        indentation();
        output += "<?";
        output += stringValue(node->getNodeName());
        const std::string value = stringValue(node->getNodeValue());
        if (!value.empty()) {
            output += ' ';
            output += value;
        }
        output += "?>";
        if (indent) output += '\n';
    }
}

} // namespace

void IdentityTransformer::transform(
    const ::jxx::Ptr<::jxx::ext::xml::transform::Source>& source,
    const ::jxx::Ptr<::jxx::ext::xml::transform::Result>& result) {
    if (source == nullptr || result == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto domSource =
        ::jxx::CAST<::jxx::ext::xml::transform::dom::DOMSource>(source);
    const auto streamResult =
        ::jxx::CAST<::jxx::ext::xml::transform::stream::StreamResult>(result);
    if (domSource == nullptr || streamResult == nullptr) {
        throw ::jxx::ext::xml::transform::TransformerException(
            ::jxx::NEW<::jxx::lang::String>(
                "identity transformer requires DOMSource and StreamResult"));
    }
    if (domSource->getNode() == nullptr) {
        throw ::jxx::ext::xml::transform::TransformerException(
            ::jxx::NEW<::jxx::lang::String>("DOMSource node is null"));
    }

    const auto omit = getOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::OMIT_XML_DECLARATION());
    const auto indentValue = getOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::INDENT());
    const auto encoding = getOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::ENCODING());

    const bool omitDeclaration =
        omit != nullptr && omit->utf8() == "yes";
    const bool indent =
        indentValue != nullptr && indentValue->utf8() == "yes";
    const std::string encodingName =
        encoding == nullptr ? "UTF-8" : encoding->utf8();

    std::string xml;
    if (!omitDeclaration) {
        xml = "<?xml version=\"1.0\" encoding=\"" +
            encodingName + "\"?>";
        if (indent) xml += '\n';
    }
    serializeNode(domSource->getNode(), xml, indent, 0);

    const auto text = ::jxx::NEW<::jxx::lang::String>(xml);
    if (streamResult->getWriter() != nullptr) {
        streamResult->getWriter()->write(text);
        streamResult->getWriter()->flush();
        return;
    }

    ::jxx::Ptr<::jxx::io::OutputStream> output =
        streamResult->getOutputStream();
    bool closeOutput = false;
    if (output == nullptr && streamResult->getSystemId() != nullptr) {
        output = ::jxx::NEW<::jxx::io::FileOutputStream>(
            streamResult->getSystemId());
        closeOutput = true;
    }
    if (output == nullptr) {
        throw ::jxx::ext::xml::transform::TransformerException(
            ::jxx::NEW<::jxx::lang::String>(
                "StreamResult has no writer, output stream, or system ID"));
    }

    try {
        const auto bytes = text->getBytes(
            ::jxx::NEW<::jxx::lang::String>(encodingName));
        output->write(bytes);
        output->flush();
        if (closeOutput) output->close();
    } catch (...) {
        if (closeOutput) {
            try { output->close(); } catch (...) {}
        }
        throw;
    }
}

void IdentityTransformer::setOutputProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (name == nullptr || value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    outputProperties_[name->utf8()] = value;
}

::jxx::Ptr<::jxx::lang::String> IdentityTransformer::getOutputProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    const auto iterator = outputProperties_.find(name->utf8());
    return iterator == outputProperties_.end() ? nullptr : iterator->second;
}

void IdentityTransformer::setOutputProperties(
    const ::jxx::Ptr<::jxx::util::Properties>& properties) {
    outputProperties_.clear();
    if (properties == nullptr) return;
    const auto names = properties->stringPropertyNames();
    if (names == nullptr) return;
    const auto iterator = names->iterator();
    while (iterator->hasNext()) {
        const auto name = iterator->next();
        setOutputProperty(name, properties->getProperty(name));
    }
}

::jxx::Ptr<::jxx::util::Properties>
IdentityTransformer::getOutputProperties() const {
    const auto result = ::jxx::NEW<::jxx::util::Properties>();
    for (const auto& entry : outputProperties_) {
        result->setProperty(
            ::jxx::NEW<::jxx::lang::String>(entry.first),
            entry.second);
    }
    return result;
}

void IdentityTransformer::setParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (name == nullptr || value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    parameters_[name->utf8()] = value;
}

::jxx::Ptr<::jxx::lang::Object> IdentityTransformer::getParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    const auto iterator = parameters_.find(name->utf8());
    return iterator == parameters_.end() ? nullptr : iterator->second;
}

void IdentityTransformer::clearParameters() {
    parameters_.clear();
}

void IdentityTransformer::reset() {
    outputProperties_.clear();
    parameters_.clear();
}

} // namespace jxx::ext::xml::transform::internal
