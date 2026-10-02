#pragma once

#include "ext/net/jxx.ext.net.ServerSocketFactory.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::net::ssl {

class SSLServerSocketFactory
    : public ::jxx::lang::ClassBase<
          SSLServerSocketFactory,
          ::jxx::ext::net::ServerSocketFactory> {
public:
    using JxxSuper = ::jxx::ext::net::ServerSocketFactory;
    using Super = ::jxx::lang::ClassBase<
        SSLServerSocketFactory,
        JxxSuper>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;
    using JxxSuper::createServerSocket;

    ~SSLServerSocketFactory() override = default;

    static ::jxx::Ptr<::jxx::ext::net::ServerSocketFactory>
    getDefault();

    virtual ::jxx::Ptr<StringArray>
    getDefaultCipherSuites() const = 0;

    virtual ::jxx::Ptr<StringArray>
    getSupportedCipherSuites() const = 0;

protected:
    SSLServerSocketFactory() = default;
};

} // namespace jxx::ext::net::ssl
