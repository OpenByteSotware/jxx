#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.ManagerFactoryParameters.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/jxx.security.KeyStore.h"

namespace jxx::ext::net::ssl {

class KeyStoreBuilderParameters final
    : public ::jxx::lang::ClassBase<
          KeyStoreBuilderParameters,
          ::jxx::lang::Object,
          ManagerFactoryParameters> {
public:
    using BuilderArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::security::KeyStore::Builder>,
        1U>;

    explicit KeyStoreBuilderParameters(
        const ::jxx::Ptr<::jxx::security::KeyStore::Builder>& builder);

    explicit KeyStoreBuilderParameters(
        const ::jxx::Ptr<BuilderArray>& builders);

    ::jxx::Ptr<BuilderArray> getParameters() const;

private:
    ::jxx::Ptr<BuilderArray> builders_;
};

} // namespace jxx::ext::net::ssl
