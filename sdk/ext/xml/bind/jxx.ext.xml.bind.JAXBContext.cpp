#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"

#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingRegistry.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "lang/jxx.lang.String.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind {

JAXBContext::JAXBContext(
    const ::jxx::Ptr<DescriptorArray>& descriptors)
    : descriptors_(descriptors) {
    if (descriptors_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (descriptors_->length == 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    for (::jxx::lang::jint index = 0;
         index < descriptors_->length;
         ++index) {
        if ((*descriptors_)[index] == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
    }
}

::jxx::Ptr<JAXBContext> JAXBContext::newInstance(
    const ::jxx::Ptr<ClassArray>& classesToBeBound) {
    if (classesToBeBound == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (classesToBeBound->length == 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    auto descriptors = ::jxx::NEW<DescriptorArray>(classesToBeBound->length);
    for (::jxx::lang::jint index = 0;
         index < classesToBeBound->length;
         ++index) {
        const auto type = (*classesToBeBound)[index];
        if (type == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        const auto descriptor =
            metadata::BindingRegistry::descriptorFor(type);
        if (descriptor == nullptr) {
            throw JAXBException(::jxx::NEW<::jxx::lang::String>(
                std::string("No binding descriptor registered for ") +
                type->getName()->utf8()));
        }
        (*descriptors)[index] = descriptor;
    }
    return ::jxx::NEW<JAXBContext>(descriptors);
}

::jxx::Ptr<JAXBContext> JAXBContext::newInstance(
    const ::jxx::Ptr<DescriptorArray>& descriptors) {
    return ::jxx::NEW<JAXBContext>(descriptors);
}

::jxx::Ptr<Unmarshaller> JAXBContext::createUnmarshaller() {
    return ::jxx::CAST<Unmarshaller>(
        ::jxx::NEW<internal::DomUnmarshaller>(descriptors_));
}

::jxx::Ptr<Marshaller> JAXBContext::createMarshaller() {
    return ::jxx::CAST<Marshaller>(
        ::jxx::NEW<internal::XmlMarshaller>());
}

} // namespace jxx::ext::xml::bind
