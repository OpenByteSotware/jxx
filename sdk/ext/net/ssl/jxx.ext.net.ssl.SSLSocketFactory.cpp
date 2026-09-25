#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
namespace jxx::ext::net::ssl {::jxx::Ptr<::jxx::ext::net::SocketFactory> SSLSocketFactory::getDefault(){return SSLContext::getDefault()->getSocketFactory();}::jxx::Ptr<::jxx::net::Socket> SSLSocketFactory::createSocket(const ::jxx::Ptr<::jxx::net::Socket>&,const ::jxx::Ptr<::jxx::io::InputStream>&,::jxx::lang::jbool){throw ::jxx::lang::UnsupportedOperationException();}}
