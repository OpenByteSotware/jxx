#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::net { class InetAddress; class ServerSocket; }
namespace jxx::ext::net::ssl {

class SSLServerSocketFactory
    : public ::jxx::lang::ClassBase<
          SSLServerSocketFactory,
          ::jxx::lang::Object> {
public:
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    ~SSLServerSocketFactory() override = default;
    static ::jxx::Ptr<SSLServerSocketFactory> getDefault();
    virtual ::jxx::Ptr<StringArray> getDefaultCipherSuites() const = 0;
    virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites() const = 0;
    virtual ::jxx::Ptr<::jxx::net::ServerSocket> createServerSocket() = 0;
    virtual ::jxx::Ptr<::jxx::net::ServerSocket> createServerSocket(
        ::jxx::lang::jint port) = 0;
    virtual ::jxx::Ptr<::jxx::net::ServerSocket> createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog) = 0;
    virtual ::jxx::Ptr<::jxx::net::ServerSocket> createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address) = 0;

protected:
    SSLServerSocketFactory() = default;
};

} // namespace jxx::ext::net::ssl
