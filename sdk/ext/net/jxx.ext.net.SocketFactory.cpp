#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/internal.DefaultSocketFactory.h"
namespace jxx::ext::net { ::jxx::Ptr<SocketFactory> SocketFactory::getDefault(){static auto f=::jxx::NEW<::jxx::ext::net::internal::DefaultSocketFactory>();return f;} }
