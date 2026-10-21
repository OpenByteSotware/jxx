#include "ext/security/jxx.ext.security.ClassInitialization.h"
#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"

namespace jxx::ext::security
{
void initialize()
{
    ::jxx::ext::security::auth::initialize();
    ::jxx::ext::security::cert::initialize();
}

} // namespace jxx::ext::security
