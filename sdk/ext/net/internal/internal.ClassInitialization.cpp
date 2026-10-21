#include <mutex>
#include "ext/net/internal/internal.ClassInitialization.h"
#include "ext/net/internal/internal.DefaultServerSocketFactory.h"
#include "ext/net/internal/internal.DefaultSocketFactory.h"
#include "ext/net/jxx.ext.net.ServerSocketFactory.h"
#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ClassInitialization.h"
namespace jxx::ext::net::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DefaultServerSocketFactory::JxxClassInfoMarker::Class();
        (void)DefaultSocketFactory::JxxClassInfoMarker::Class();
        (void)ServerSocketFactory::JxxClassInfoMarker::Class();
        (void)SocketFactory::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::ext::net::ssl::initialize();
}

} // namespace jxx::ext::net::internal
