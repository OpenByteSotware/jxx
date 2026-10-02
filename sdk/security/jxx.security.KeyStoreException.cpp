#include "security/jxx.security.KeyStoreException.h"

namespace jxx::security {

KeyStoreException::KeyStoreException() = default;
KeyStoreException::KeyStoreException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}
KeyStoreException::KeyStoreException(const char* message)
    : Super(message) {
}
::jxx::Ptr<::jxx::lang::Object> KeyStoreException::cloneImpl() const {
    return ::jxx::NEW<KeyStoreException>(*this);
}
const char* KeyStoreException::typeName() const noexcept {
    return "jxx.security.KeyStoreException";
}

} // namespace jxx::security
