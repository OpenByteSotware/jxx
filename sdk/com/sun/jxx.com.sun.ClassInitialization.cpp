#include "com/sun/jxx.com.sun.ClassInitialization.h"
#include "com/sun/net/jxx.com.sun.net.ClassInitialization.h"

namespace com::sun
{
void initialize()
{
    ::com::sun::net::initialize();
}

} // namespace com::sun
