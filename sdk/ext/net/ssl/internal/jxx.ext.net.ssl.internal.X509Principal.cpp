#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl::internal {
X509Principal::X509Principal(
    const ::jxx::Ptr<::jxx::lang::String>& name) : name_(name) {
    if (name_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> X509Principal::getName() const { return name_; }
::jxx::Ptr<::jxx::lang::String> X509Principal::toString() const { return name_; }
::jxx::lang::jbool X509Principal::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto principal = ::jxx::CAST<X509Principal>(other);
    return principal != nullptr && name_->equals(principal->name_);
}
::jxx::lang::jint X509Principal::hashCode() const { return name_->hashCode(); }
} // namespace jxx::ext::net::ssl::internal
