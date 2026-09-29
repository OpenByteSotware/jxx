#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"

#include "io/jxx.io.ByteArrayInputStream.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::net::ssl::internal {

::jxx::Ptr<::jxx::io::InputStream>
OpenSslHttpsURLConnection::getInputStream() {
    connect();
    if (responseBody_ == nullptr) {
        executeRequest();
    }
    return ::jxx::NEW<
        ::jxx::io::ByteArrayInputStream>(
            responseBody_);
}

::jxx::lang::jint
OpenSslHttpsURLConnection::getResponseCode() {
    if (responseBody_ == nullptr) {
        executeRequest();
    }
    return responseCode_;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslHttpsURLConnection::getResponseMessage() {
    if (responseBody_ == nullptr) {
        executeRequest();
    }
    return responseMessage_;
}

void OpenSslHttpsURLConnection::disconnect() {
    releaseSocket();
    connected_ = false;
    responseBody_ = nullptr;
}

::jxx::lang::jbool
OpenSslHttpsURLConnection::usingProxy()
    const {
    return false;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslHttpsURLConnection::getCipherSuite()
    const {
    auto* self =
        const_cast<
            OpenSslHttpsURLConnection*>(
                this);
    self->connect();
    return cipherSuite_;
}

::jxx::Ptr<
    OpenSslHttpsURLConnection::
        CertificateArray>
OpenSslHttpsURLConnection::
getLocalCertificates() const {
    auto* self =
        const_cast<
            OpenSslHttpsURLConnection*>(
                this);
    self->connect();
    return localCertificates_;
}

::jxx::Ptr<
    OpenSslHttpsURLConnection::
        CertificateArray>
OpenSslHttpsURLConnection::
getServerCertificates() const {
    auto* self =
        const_cast<
            OpenSslHttpsURLConnection*>(
                this);
    self->connect();
    return serverCertificates_;
}

} // namespace jxx::ext::net::ssl::internal
