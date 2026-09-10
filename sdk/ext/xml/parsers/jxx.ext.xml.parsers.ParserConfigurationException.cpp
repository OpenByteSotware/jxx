#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::parsers {

::jxx::Ptr<::jxx::lang::ClassAny> ParserConfigurationException::Class() {
    return JxxClassInfoMarker::Class();
}

ParserConfigurationException::ParserConfigurationException()
    : JxxSuper() {
}

ParserConfigurationException::ParserConfigurationException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {
}

ParserConfigurationException::~ParserConfigurationException() = default;

::jxx::Ptr<::jxx::lang::Object>
ParserConfigurationException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<ParserConfigurationException>(*this));
}

const char* ParserConfigurationException::typeName() const noexcept {
    return "ParserConfigurationException";
}

} // namespace jxx::ext::xml::parsers
