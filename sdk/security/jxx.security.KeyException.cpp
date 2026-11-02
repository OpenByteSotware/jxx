#include "security/jxx.security.KeyException.h"

namespace jxx::security {

KeyException::KeyException() = default;
KeyException::KeyException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}
KeyException::KeyException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : Super(message, cause) {}
KeyException::KeyException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : Super(cause) {}
KeyException::KeyException(const char* message)
    : Super(message) {}
::jxx::Ptr<::jxx::lang::Object> KeyException::cloneImpl() const {
    return ::jxx::NEW<KeyException>(*this);
}
const char* KeyException::typeName() const noexcept {
    return "jxx.security.KeyException";
}

} // namespace jxx::security
