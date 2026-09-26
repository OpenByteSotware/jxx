#include "ext/net/ssl/jxx.ext.net.ssl.ExtendedSSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIServerName.h"
#include "util/jxx.util.ArrayList.h"
namespace jxx::ext::net::ssl {
::jxx::Ptr<::jxx::util::List<SNIServerName>>
ExtendedSSLSession::getRequestedServerNames() const {
    return ::jxx::CAST<::jxx::util::List<SNIServerName>>(
        ::jxx::NEW<::jxx::util::ArrayList<SNIServerName>>());
}
} // namespace jxx::ext::net::ssl
