#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElementI.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::internal {
std::string XmlMarshaller::escape_(const std::string& value, ::jxx::lang::jbool attribute) {
    std::string out;
    for (char current : value) {
        switch (current) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += attribute ? "&quot;" : "\""; break;
            case '\'': out += attribute ? "&apos;" : "'"; break;
            default: out.push_back(current); break;
        }
    }
    return out;
}

std::string XmlMarshaller::qualifiedName_(
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace) {
    if (nameSpace == nullptr || nameSpace->isEmpty()) return localName->utf8();
    return "ns:" + localName->utf8();
}

std::string XmlMarshaller::writeBinding_(
    const ::jxx::Ptr<::jxx::lang::Object>& value,
    const ::jxx::Ptr<metadata::MarshallingBinding>& binding) {
    const auto name = qualifiedName_(binding->localName(), binding->nameSpace());
    if (binding->kind() == metadata::PropertyBinding::Kind::VALUE ||
        binding->isTextValue()) {
        const auto lexicalValue = binding->formatter()->format(value);
        if (binding->fixedValue() != nullptr &&
            lexicalValue->utf8() != binding->fixedValue()->utf8()) {
            throw JAXBException(
                ::jxx::NEW<::jxx::lang::String>("Value does not match fixed XML value"));
        }
        if (binding->omitDefault() &&
            binding->defaultValue() != nullptr &&
            lexicalValue->utf8() == binding->defaultValue()->utf8()) {
            return "";
        }
        if (binding->kind() == metadata::PropertyBinding::Kind::VALUE) {
            return escape_(lexicalValue->utf8(), false);
        }
        return "<" + name + ">" +
            escape_(lexicalValue->utf8(), false) +
            "</" + name + ">";
    }
    return writeObject_(value, binding->childDescriptor());
}

std::string XmlMarshaller::writeObject_(
    const ::jxx::Ptr<::jxx::lang::Object>& object,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) {
    if (object == nullptr || descriptor == nullptr) throw ::jxx::lang::NullPointerException();
    const auto rootName = qualifiedName_(
        descriptor->rootName(), descriptor->rootNamespace());
    std::string output = "<" + rootName;
    if (descriptor->rootNamespace() != nullptr &&
        !descriptor->rootNamespace()->isEmpty()) {
        output += " xmlns:ns=\"" +
            escape_(descriptor->rootNamespace()->utf8(), true) + "\"";
    }
    const auto bindings = descriptor->bindings();
    for (::jxx::lang::jint index = 0; index < bindings->length; ++index) {
        const auto binding = (*bindings)[index];
        if (binding == nullptr || binding->kind() != metadata::PropertyBinding::Kind::ATTRIBUTE) continue;
        const auto value = binding->reader()->read(object);
        if (value == nullptr) {
            if (binding->required()) throw JAXBException(::jxx::NEW<::jxx::lang::String>("Missing required attribute"));
            continue;
        }
        const auto lexicalValue = binding->formatter()->format(value);
        if (binding->fixedValue() != nullptr &&
            lexicalValue->utf8() != binding->fixedValue()->utf8()) {
            throw JAXBException(
                ::jxx::NEW<::jxx::lang::String>("Value does not match fixed XML value"));
        }
        if (binding->omitDefault() &&
            binding->defaultValue() != nullptr &&
            lexicalValue->utf8() == binding->defaultValue()->utf8()) {
            continue;
        }
        output += " " + qualifiedName_(binding->localName(), binding->nameSpace()) + "=\"" +
            escape_(lexicalValue->utf8(), true) + "\"";
    }
    output += ">";
    if (descriptor->isMixed()) {
        const auto reader = descriptor->mixedReader();
        for (::jxx::lang::jint index = 0;
             index < reader->size(object);
             ++index) {
            const auto value = reader->value(object, index);
            if (value == nullptr) continue;
            if (reader->kind(object, index) ==
                metadata::MixedContentReader::Kind::TEXT) {
                const auto text =
                    std::dynamic_pointer_cast<::jxx::lang::String>(value);
                if (text != nullptr) output += escape_(text->utf8(), false);
            } else {
                for (::jxx::lang::jint bindingIndex = 0;
                     bindingIndex < bindings->length;
                     ++bindingIndex) {
                    const auto binding = (*bindings)[bindingIndex];
                    if (binding != nullptr &&
                        binding->kind() == metadata::PropertyBinding::Kind::ELEMENT) {
                        output += writeBinding_(value, binding);
                        break;
                    }
                }
            }
        }
    } else {
    for (::jxx::lang::jint index = 0; index < bindings->length; ++index) {
        const auto binding = (*bindings)[index];
        if (binding == nullptr || binding->kind() == metadata::PropertyBinding::Kind::ATTRIBUTE) continue;
        const auto value = binding->reader()->read(object);
        if (value == nullptr) {
            if (binding->required()) throw JAXBException(::jxx::NEW<::jxx::lang::String>("Missing required element"));
            if (binding->nillable()) output += "<" + binding->localName()->utf8() + " xsi:nil=\"true\"/>";
            continue;
        }
        if (binding->repeated()) {
            const auto reader = binding->repeatedReader();
            const auto count = reader->size(value);
            for (::jxx::lang::jint itemIndex = 0;
                 itemIndex < count;
                 ++itemIndex) {
                const auto item = reader->get(value, itemIndex);
                if (item != nullptr) output += writeBinding_(item, binding);
            }
        } else {
            output += writeBinding_(value, binding);
        }
    }
    }
    output += "</" + rootName + ">";
    if (descriptor->lifecycle() != nullptr) {
        descriptor->lifecycle()->afterMarshal(object);
    }
    return output;
}

std::string XmlMarshaller::writeElement_(
    const ::jxx::Ptr<::jxx::ext::xml::bind::JAXBElementI>& element,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) {
    if (element == nullptr || descriptor == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto name = element->getName();
    const auto localName = name->getLocalPart()->utf8();
    const auto nameSpace = name->getNamespaceURI()->utf8();
    const auto prefix = name->getPrefix()->utf8();
    const auto qualifiedName = prefix.empty()
        ? localName
        : prefix + ":" + localName;
    if (element->isNil()) {
        std::string output = "<" + qualifiedName;
        if (!nameSpace.empty()) {
            output += prefix.empty()
                ? " xmlns=\"" + escape_(nameSpace, true) + "\""
                : " xmlns:" + prefix + "=\"" + escape_(nameSpace, true) + "\"";
        }
        output += " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:nil=\"true\"/>";
        return output;
    }
    auto output = writeObject_(element->getValueObject(), descriptor);
    const auto descriptorName = descriptor->rootName()->utf8();
    const auto descriptorOpen = "<" + descriptorName;
    const auto descriptorClose = "</" + descriptorName + ">";
    const auto replacementOpen = "<" + qualifiedName;
    if (output.rfind(descriptorOpen, 0U) == 0U) {
        output.replace(0U, descriptorOpen.size(), replacementOpen);
    }
    const auto closePosition = output.rfind(descriptorClose);
    if (closePosition != std::string::npos) {
        output.replace(closePosition, descriptorClose.size(), "</" + qualifiedName + ">");
    }
    if (!nameSpace.empty()) {
        const auto insertion = output.find('>');
        if (insertion != std::string::npos) {
            const auto declaration = prefix.empty()
                ? " xmlns=\"" + escape_(nameSpace, true) + "\""
                : " xmlns:" + prefix + "=\"" + escape_(nameSpace, true) + "\"";
            output.insert(insertion, declaration);
        }
    }
    return output;
}

::jxx::Ptr<::jxx::lang::String> XmlMarshaller::marshal(
    const ::jxx::Ptr<::jxx::lang::Object>& object,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) {
    return ::jxx::NEW<::jxx::lang::String>(writeObject_(object, descriptor));
}
void XmlMarshaller::marshal(
    const ::jxx::Ptr<::jxx::lang::Object>& object,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor,
    const ::jxx::Ptr<::jxx::io::Writer>& writer) {
    if (writer == nullptr) throw ::jxx::lang::NullPointerException();
    writer->write(marshal(object, descriptor));
    writer->flush();
}
::jxx::Ptr<::jxx::lang::String> XmlMarshaller::marshalElement(
    const ::jxx::Ptr<::jxx::ext::xml::bind::JAXBElementI>& element,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) {
    return ::jxx::NEW<::jxx::lang::String>(writeElement_(element, descriptor));
}

void XmlMarshaller::marshalElement(
    const ::jxx::Ptr<::jxx::ext::xml::bind::JAXBElementI>& element,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor,
    const ::jxx::Ptr<::jxx::io::Writer>& writer) {
    if (writer == nullptr) throw ::jxx::lang::NullPointerException();
    writer->write(marshalElement(element, descriptor));
    writer->flush();
}

} // namespace jxx::ext::xml::bind::internal
