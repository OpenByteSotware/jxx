#include "ext/xml/parsers/jxx.ext.xml.parsers.FactoryConfigurationError.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::parsers {

::jxx::Ptr<::jxx::lang::ClassAny> FactoryConfigurationError::Class() {
    return JxxClassInfoMarker::Class();
}

FactoryConfigurationError::FactoryConfigurationError()
    : JxxSuper() {
}

FactoryConfigurationError::FactoryConfigurationError(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {
}

FactoryConfigurationError::~FactoryConfigurationError() = default;

::jxx::Ptr<::jxx::lang::Object>
FactoryConfigurationError::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<FactoryConfigurationError>(*this));
}

const char* FactoryConfigurationError::typeName() const noexcept {
    return "FactoryConfigurationError";
}

} // namespace jxx::ext::xml::parsers
