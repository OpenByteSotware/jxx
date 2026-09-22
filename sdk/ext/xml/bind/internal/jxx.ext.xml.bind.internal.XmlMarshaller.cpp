#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
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
std::string XmlMarshaller::writeObject_(
    const ::jxx::Ptr<::jxx::lang::Object>& object,
    const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) {
    if (object == nullptr || descriptor == nullptr) throw ::jxx::lang::NullPointerException();
    std::string output = "<" + descriptor->rootName()->utf8();
    const auto bindings = descriptor->bindings();
    for (::jxx::lang::jint index = 0; index < bindings->length; ++index) {
        const auto binding = (*bindings)[index];
        if (binding == nullptr || binding->kind() != metadata::PropertyBinding::Kind::ATTRIBUTE) continue;
        const auto value = binding->reader()->read(object);
        if (value == nullptr) {
            if (binding->required()) throw JAXBException(::jxx::NEW<::jxx::lang::String>("Missing required attribute"));
            continue;
        }
        output += " " + binding->localName()->utf8() + "=\"" +
            escape_(binding->formatter()->format(value)->utf8(), true) + "\"";
    }
    output += ">";
    for (::jxx::lang::jint index = 0; index < bindings->length; ++index) {
        const auto binding = (*bindings)[index];
        if (binding == nullptr || binding->kind() == metadata::PropertyBinding::Kind::ATTRIBUTE) continue;
        const auto value = binding->reader()->read(object);
        if (value == nullptr) {
            if (binding->required()) throw JAXBException(::jxx::NEW<::jxx::lang::String>("Missing required element"));
            if (binding->nillable()) output += "<" + binding->localName()->utf8() + " xsi:nil=\"true\"/>";
            continue;
        }
        if (binding->kind() == metadata::PropertyBinding::Kind::VALUE) {
            output += escape_(binding->formatter()->format(value)->utf8(), false);
        } else if (binding->isTextValue()) {
            output += "<" + binding->localName()->utf8() + ">" +
                escape_(binding->formatter()->format(value)->utf8(), false) +
                "</" + binding->localName()->utf8() + ">";
        } else {
            output += writeObject_(value, binding->childDescriptor());
        }
    }
    output += "</" + descriptor->rootName()->utf8() + ">";
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
} // namespace jxx::ext::xml::bind::internal
