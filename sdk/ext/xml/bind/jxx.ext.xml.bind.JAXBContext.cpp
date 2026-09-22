#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"

#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"
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
    const ::jxx::Ptr<DescriptorArray>& descriptors) {
    return ::jxx::NEW<JAXBContext>(descriptors);
}

::jxx::Ptr<Unmarshaller> JAXBContext::createUnmarshaller() {
    return ::jxx::CAST<Unmarshaller>(
        ::jxx::NEW<internal::DomUnmarshaller>(descriptors_));
}

} // namespace jxx::ext::xml::bind
