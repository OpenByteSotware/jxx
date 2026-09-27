#include "security/jxx.security.NoSuchAlgorithmException.h"

namespace jxx::security {

NoSuchAlgorithmException::NoSuchAlgorithmException() = default;
NoSuchAlgorithmException::NoSuchAlgorithmException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}
NoSuchAlgorithmException::NoSuchAlgorithmException(const char* message)
    : Super(message) {
}
::jxx::Ptr<::jxx::lang::Object>
NoSuchAlgorithmException::cloneImpl() const {
    return ::jxx::NEW<NoSuchAlgorithmException>(*this);
}
const char* NoSuchAlgorithmException::typeName() const noexcept {
    return "jxx.security.NoSuchAlgorithmException";
}

} // namespace jxx::security
