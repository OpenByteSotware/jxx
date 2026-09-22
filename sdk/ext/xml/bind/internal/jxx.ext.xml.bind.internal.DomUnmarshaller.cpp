#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"

#include <vector>

#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace jxx::ext::xml::bind::internal {
namespace {

::jxx::lang::jbool equalString(
    const ::jxx::Ptr<::jxx::lang::String>& left,
    const ::jxx::Ptr<::jxx::lang::String>& right) {
    return left == right ||
        (left != nullptr && right != nullptr &&
         left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}

::jxx::Ptr<::jxx::org::w3c::dom::Element> elementFromNode(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    if (node == nullptr) throw ::jxx::lang::NullPointerException();
    if (node->getNodeType() == ::jxx::org::w3c::dom::Node::DOCUMENT_NODE) {
        const auto document =
            std::dynamic_pointer_cast<::jxx::org::w3c::dom::Document>(node);
        return document == nullptr ? nullptr : document->getDocumentElement();
    }
    return std::dynamic_pointer_cast<::jxx::org::w3c::dom::Element>(node);
}

} // namespace

DomUnmarshaller::DomUnmarshaller(
    const ::jxx::Ptr<DescriptorArray>& descriptors)
    : descriptors_(descriptors) {
    if (descriptors_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::Object> DomUnmarshaller::unmarshal(
    const ::jxx::Ptr<::jxx::io::File>& file) {
    if (file == nullptr) throw ::jxx::lang::NullPointerException();
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    return unmarshal(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(builder->parse(file)));
}

::jxx::Ptr<::jxx::lang::Object> DomUnmarshaller::unmarshal(
    const ::jxx::Ptr<::jxx::io::InputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    return unmarshal(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(builder->parse(input)));
}

::jxx::Ptr<::jxx::lang::Object> DomUnmarshaller::unmarshal(
    const ::jxx::Ptr<::jxx::io::Reader>& reader) {
    if (reader == nullptr) throw ::jxx::lang::NullPointerException();
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto source =
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader);
    return unmarshal(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(builder->parse(source)));
}

::jxx::Ptr<::jxx::lang::Object> DomUnmarshaller::unmarshal(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    const auto element = elementFromNode(node);
    if (element == nullptr) {
        throw UnmarshalException(
            ::jxx::NEW<::jxx::lang::String>("Root node is not an element"));
    }
    const auto descriptor = findRoot_(element);
    if (descriptor == nullptr) {
        throw UnmarshalException(
            ::jxx::NEW<::jxx::lang::String>("No root binding descriptor"));
    }
    return readObject_(element, descriptor, nullptr);
}


::jxx::Ptr<::jxx::ext::xml::bind::JAXBElementI>
DomUnmarshaller::unmarshalElement(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    const auto element = elementFromNode(node);
    if (element == nullptr) {
        throw UnmarshalException(
            ::jxx::NEW<::jxx::lang::String>("Root node is not an element"));
    }
    const auto descriptor = findRoot_(element);
    if (descriptor == nullptr || descriptor->wrapperFactory() == nullptr) {
        throw UnmarshalException(
            ::jxx::NEW<::jxx::lang::String>("No root element wrapper factory"));
    }
    const auto value = readObject_(element, descriptor, nullptr);
    const auto name = ::jxx::NEW<::jxx::ext::xml::namespace_::QName>(
        namespace_(element), localName_(element));
    const auto xsiNamespace = ::jxx::NEW<::jxx::lang::String>(
        "http://www.w3.org/2001/XMLSchema-instance");
    const auto nilName = ::jxx::NEW<::jxx::lang::String>("nil");
    const auto nil = element->hasAttributeNS(xsiNamespace, nilName) &&
        element->getAttributeNS(xsiNamespace, nilName)->utf8() == "true";
    return descriptor->wrapperFactory()->wrap(name, value, nil);
}

::jxx::Ptr<metadata::BindingDescriptor> DomUnmarshaller::findRoot_(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& element) const {
    const auto local = localName_(element);
    const auto nameSpace = namespace_(element);
    for (::jxx::lang::jint index = 0;
         index < descriptors_->length;
         ++index) {
        const auto descriptor = (*descriptors_)[index];
        if (equalString(descriptor->rootName(), local) &&
            equalString(descriptor->rootNamespace(), nameSpace)) {
            return descriptor;
        }
    }
    return nullptr;
}

::jxx::Ptr<::jxx::lang::Object> DomUnmarshaller::readObject_(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& element,
    const ::jxx::Ptr<metadata::BindingDescriptor>& descriptor,
    const ::jxx::Ptr<::jxx::lang::Object>& parent) const {
    const auto target = descriptor->factory()->create();
    if (target == nullptr) {
        throw UnmarshalException(
            ::jxx::NEW<::jxx::lang::String>("Object factory returned null"));
    }
    if (descriptor->lifecycle() != nullptr) {
        descriptor->lifecycle()->beforeUnmarshal(target, parent);
    }

    const auto properties = descriptor->properties();
    std::vector<::jxx::lang::jbool> seen(
        static_cast<std::size_t>(properties->length), false);

    for (::jxx::lang::jint index = 0;
         index < properties->length;
         ++index) {
        const auto property = (*properties)[index];
        if (property == nullptr ||
            property->kind() != metadata::PropertyBinding::Kind::ATTRIBUTE) {
            continue;
        }
        const auto attribute = element->getAttributeNS(
            property->nameSpace(), property->localName());
        if (attribute != nullptr && !attribute->isEmpty()) {
            const auto value = property->converter()->convert(attribute);
            property->writer()->write(target, value);
            seen[static_cast<std::size_t>(index)] = true;
        }
    }

    const auto children = element->getChildNodes();
    if (children != nullptr) {
        for (::jxx::lang::jint index = 0;
             index < children->getLength();
             ++index) {
            const auto child = children->item(index);
            if (child == nullptr) continue;
            if (descriptor->isMixed() &&
                (child->getNodeType() ==
                    ::jxx::org::w3c::dom::Node::TEXT_NODE ||
                 child->getNodeType() ==
                    ::jxx::org::w3c::dom::Node::CDATA_SECTION_NODE)) {
                const auto text = child->getTextContent();
                if (text != nullptr && !text->isEmpty()) {
                    descriptor->mixedWriter()->addText(target, text);
                }
                continue;
            }
            if (child->getNodeType() !=
                ::jxx::org::w3c::dom::Node::ELEMENT_NODE) {
                continue;
            }
            const auto property = descriptor->findElement(
                localName_(child), namespace_(child));
            if (property == nullptr) continue;
            for (::jxx::lang::jint propertyIndex = 0;
                 propertyIndex < properties->length;
                 ++propertyIndex) {
                if ((*properties)[propertyIndex] == property) {
                    seen[static_cast<std::size_t>(propertyIndex)] = true;
                    break;
                }
            }
            ::jxx::Ptr<::jxx::lang::Object> value;
            if (property->isTextValue()) {
                value = property->converter()->convert(child->getTextContent());
            } else {
                const auto childElement =
                    std::dynamic_pointer_cast<::jxx::org::w3c::dom::Element>(child);
                value = readObject_(childElement, property->childDescriptor(), target);
            }
            property->writer()->write(target, value);
        }
    }

    for (::jxx::lang::jint index = 0;
         index < properties->length;
         ++index) {
        const auto property = (*properties)[index];
        if (property != nullptr && property->required() &&
            !seen[static_cast<std::size_t>(index)]) {
            throw UnmarshalException(
                ::jxx::NEW<::jxx::lang::String>("Missing required XML value"));
        }
    }
    if (descriptor->lifecycle() != nullptr) {
        descriptor->lifecycle()->afterUnmarshal(target, parent);
    }
    return target;
}

::jxx::Ptr<::jxx::lang::String> DomUnmarshaller::localName_(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    const auto local = node->getLocalName();
    return local != nullptr && !local->isEmpty()
        ? local
        : node->getNodeName();
}

::jxx::Ptr<::jxx::lang::String> DomUnmarshaller::namespace_(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) {
    const auto value = node->getNamespaceURI();
    return value == nullptr
        ? ::jxx::NEW<::jxx::lang::String>("")
        : value;
}

} // namespace jxx::ext::xml::bind::internal
