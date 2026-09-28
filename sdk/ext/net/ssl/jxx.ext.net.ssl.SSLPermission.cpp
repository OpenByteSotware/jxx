#include "ext/net/ssl/jxx.ext.net.ssl.SSLPermission.h"

namespace jxx::ext::net::ssl {

SSLPermission::SSLPermission(
    const ::jxx::Ptr<::jxx::lang::String>& name)
    : Super(name) {
}

SSLPermission::SSLPermission(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& actions)
    : Super(name, actions) {
}

::jxx::Ptr<::jxx::lang::Object>
SSLPermission::cloneImpl() const {
    return ::jxx::NEW<SSLPermission>(*this);
}


} // namespace jxx::ext::net::ssl
