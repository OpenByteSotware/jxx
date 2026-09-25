#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.List.h"

namespace jxx::ext::net::ssl {

class SNIServerName;

class SSLParameters
    : public ::jxx::lang::ClassBase<
          SSLParameters,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SSLParameters, JxxSuper>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    SSLParameters();
    explicit SSLParameters(
        const ::jxx::Ptr<StringArray>& cipherSuites);
    SSLParameters(
        const ::jxx::Ptr<StringArray>& cipherSuites,
        const ::jxx::Ptr<StringArray>& protocols);

    ::jxx::Ptr<StringArray> getCipherSuites() const;
    void setCipherSuites(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getProtocols() const;
    void setProtocols(const ::jxx::Ptr<StringArray>& values);

    ::jxx::lang::jbool getNeedClientAuth() const;
    void setNeedClientAuth(::jxx::lang::jbool value);
    ::jxx::lang::jbool getWantClientAuth() const;
    void setWantClientAuth(::jxx::lang::jbool value);

    ::jxx::Ptr<::jxx::lang::String>
    getEndpointIdentificationAlgorithm() const;
    void setEndpointIdentificationAlgorithm(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    ::jxx::Ptr<::jxx::util::List<SNIServerName>>
    getServerNames() const;
    void setServerNames(
        const ::jxx::Ptr<::jxx::util::List<SNIServerName>>& names);

private:
    static ::jxx::Ptr<StringArray> copyOf(
        const ::jxx::Ptr<StringArray>& values);

    ::jxx::Ptr<StringArray> ciphers_;
    ::jxx::Ptr<StringArray> protocols_;
    ::jxx::lang::jbool need_ = false;
    ::jxx::lang::jbool want_ = false;
    ::jxx::Ptr<::jxx::lang::String> endpointIdentificationAlgorithm_;
    ::jxx::Ptr<::jxx::util::List<SNIServerName>> serverNames_;
};

} // namespace jxx::ext::net::ssl
