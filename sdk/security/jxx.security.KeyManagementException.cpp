#include "security/jxx.security.KeyManagementException.h"

namespace jxx::security {

KeyManagementException::KeyManagementException() = default;
KeyManagementException::KeyManagementException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}
KeyManagementException::KeyManagementException(const char* message)
    : Super(message) {
}
::jxx::Ptr<::jxx::lang::Object>
KeyManagementException::cloneImpl() const {
    return ::jxx::NEW<KeyManagementException>(*this);
}
const char* KeyManagementException::typeName() const noexcept {
    return "jxx.security.KeyManagementException";
}

} // namespace jxx::security
