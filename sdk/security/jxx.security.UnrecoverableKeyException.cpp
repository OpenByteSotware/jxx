#include "security/jxx.security.UnrecoverableKeyException.h"

namespace jxx::security {

UnrecoverableKeyException::UnrecoverableKeyException() = default;
UnrecoverableKeyException::UnrecoverableKeyException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}
UnrecoverableKeyException::UnrecoverableKeyException(const char* message)
    : Super(message) {
}
::jxx::Ptr<::jxx::lang::Object> UnrecoverableKeyException::cloneImpl() const {
    return ::jxx::NEW<UnrecoverableKeyException>(*this);
}
const char* UnrecoverableKeyException::typeName() const noexcept {
    return "jxx.security.UnrecoverableKeyException";
}

} // namespace jxx::security
