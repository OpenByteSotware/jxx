#include "security/jxx.security.UnrecoverableEntryException.h"

namespace jxx::security {

UnrecoverableEntryException::UnrecoverableEntryException() = default;
UnrecoverableEntryException::UnrecoverableEntryException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}
UnrecoverableEntryException::UnrecoverableEntryException(const char* message)
    : Super(message) {}
::jxx::Ptr<::jxx::lang::Object> UnrecoverableEntryException::cloneImpl() const {
    return ::jxx::NEW<UnrecoverableEntryException>(*this);
}
const char* UnrecoverableEntryException::typeName() const noexcept {
    return "jxx.security.UnrecoverableEntryException";
}

} // namespace jxx::security
