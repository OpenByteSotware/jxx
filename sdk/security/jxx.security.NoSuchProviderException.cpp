#include "security/jxx.security.NoSuchProviderException.h"

namespace jxx::security {

NoSuchProviderException::NoSuchProviderException() = default;
NoSuchProviderException::NoSuchProviderException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}
NoSuchProviderException::NoSuchProviderException(const char* message)
    : Super(message) {
}
::jxx::Ptr<::jxx::lang::Object>
NoSuchProviderException::cloneImpl() const {
    return ::jxx::NEW<NoSuchProviderException>(*this);
}
const char* NoSuchProviderException::typeName() const noexcept {
    return "jxx.security.NoSuchProviderException";
}

} // namespace jxx::security
