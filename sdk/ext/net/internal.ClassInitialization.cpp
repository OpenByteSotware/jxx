#include "ext/net/internal.ClassInitialization.h"

#include <mutex>

#include "ext/net/internal.DefaultServerSocketFactory.h"
#include "ext/net/internal.DefaultSocketFactory.h"
#include "ext/net/jxx.ext.net.ServerSocketFactory.h"
#include "ext/net/jxx.ext.net.SocketFactory.h"

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

} // namespace jxx::ext::net::internal
