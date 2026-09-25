#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
namespace jxx::ext::net::ssl {::jxx::Ptr<::jxx::ext::net::SocketFactory> SSLSocketFactory::getDefault(){return SSLContext::getDefault()->getSocketFactory();}}
