#include "ext/security/auth/x500/jxx.ext.security.auth.x500.X500Principal.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::security::auth::x500 {

X500Principal::X500Principal(
    const ::jxx::Ptr<::jxx::lang::String>& name)
    : name_(name) {
    if (name_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String>
X500Principal::getName() const {
    return name_;
}

::jxx::Ptr<::jxx::lang::String>
X500Principal::toString() const {
    return name_;
}

::jxx::lang::jbool X500Principal::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto principal = ::jxx::CAST<X500Principal>(other);
    return principal != nullptr && name_->equals(principal->name_);
}

::jxx::lang::jint X500Principal::hashCode() const {
    return name_->hashCode();
}

} // namespace jxx::ext::security::auth::x500
