#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSniMatcher.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
namespace jxx::ext::net::ssl::internal {
int openSslServerNameMatcherCallback(SSL* ssl,int* alert,void* argument) noexcept {
    try {
        auto* matchers=static_cast<SniMatcherList*>(argument);
        if(ssl==nullptr||matchers==nullptr||matchers->isEmpty())return SSL_TLSEXT_ERR_OK;
        const char* requested=SSL_get_servername(ssl,TLSEXT_NAMETYPE_host_name);
        if(requested==nullptr||*requested=='\0')return SSL_TLSEXT_ERR_OK;
        const auto name=::jxx::CAST<::jxx::ext::net::ssl::SNIServerName>(
            ::jxx::NEW<::jxx::ext::net::ssl::SNIHostName>(
                ::jxx::NEW<::jxx::lang::String>(requested)));
        bool applicable=false;
        for(::jxx::lang::jint i=0;i<matchers->size();++i){
            const auto matcher=matchers->get(i);
            if(matcher==nullptr||matcher->getType()!=TLSEXT_NAMETYPE_host_name)continue;
            applicable=true;
            if(matcher->matches(name))return SSL_TLSEXT_ERR_OK;
        }
        if(!applicable)return SSL_TLSEXT_ERR_OK;
        if(alert!=nullptr)*alert=SSL_AD_UNRECOGNIZED_NAME;
        return SSL_TLSEXT_ERR_ALERT_FATAL;
    }catch(...){
        if(alert!=nullptr)*alert=SSL_AD_INTERNAL_ERROR;
        return SSL_TLSEXT_ERR_ALERT_FATAL;
    }
}
void configureServerNameMatchers(SSL_CTX* context,const ::jxx::Ptr<SniMatcherList>& matchers){
    if(context==nullptr||matchers==nullptr||matchers->isEmpty())return;
    SSL_CTX_set_tlsext_servername_callback(context,openSslServerNameMatcherCallback);
    SSL_CTX_set_tlsext_servername_arg(context,matchers.get());
}
::jxx::Ptr<::jxx::util::List<::jxx::ext::net::ssl::SNIServerName>>
requestedServerNames(SSL* ssl) {
    const auto result = ::jxx::NEW<
        ::jxx::util::ArrayList<::jxx::ext::net::ssl::SNIServerName>>();
    if (ssl == nullptr) return result;
    const char* requested = SSL_get_servername(
        ssl,
        TLSEXT_NAMETYPE_host_name);
    if (requested != nullptr && *requested != '\0')
        result->add(::jxx::NEW<::jxx::ext::net::ssl::SNIHostName>(
            ::jxx::NEW<::jxx::lang::String>(requested)));
    return ::jxx::CAST<
        ::jxx::util::List<::jxx::ext::net::ssl::SNIServerName>>(result);
}

} // namespace jxx::ext::net::ssl::internal
