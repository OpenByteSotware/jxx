#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
namespace jxx::ext::net::ssl {
::jxx::Ptr<SSLServerSocketFactory> SSLServerSocketFactory::getDefault(){return SSLContext::getDefault()->getServerSocketFactory();}
}
