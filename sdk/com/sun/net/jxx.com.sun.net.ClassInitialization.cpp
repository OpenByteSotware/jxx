#include "com/sun/net/jxx.com.sun.net.ClassInitialization.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.ClassInitialization.h"

namespace com::sun::net
{
void initialize()
{
    ::jxx::com::sun::net::httpserver::initialize();
}

} // namespace com::sun::net
