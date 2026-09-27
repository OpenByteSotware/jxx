#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

GeneralSecurityException::GeneralSecurityException() = default;

GeneralSecurityException::GeneralSecurityException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

GeneralSecurityException::GeneralSecurityException(
    const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
GeneralSecurityException::cloneImpl() const {
    return ::jxx::NEW<GeneralSecurityException>(*this);
}

const char* GeneralSecurityException::typeName() const noexcept {
    return "jxx.security.GeneralSecurityException";
}

} // namespace jxx::security
