#include "ext/jxx.ext.ClassInitialization.h"
#include "ext/crypto/jxx.ext.crypto.ClassInitialization.h"
#include "ext/imageio/jxx.ext.imageio.ClassInitialization.h"
#include "ext/security/jxx.ext.security.ClassInitialization.h"
#include "ext/sound/jxx.ext.sound.ClassInitialization.h"
#include "ext/swing/jxx.ext.swing.ClassInitialization.h"
#include "ext/xml/jxx.ext.xml.ClassInitialization.h"
#include "ext/net/jxx.ext.net.ClassInitialization.h"

namespace jxx::ext
{
void initialize()
{
    ::jxx::ext::crypto::initialize();
    ::jxx::ext::imageio::initialize();
    ::jxx::ext::net::initialize();
    ::jxx::ext::security::initialize();
    ::jxx::ext::sound::initialize();
    ::jxx::ext::swing::initialize();
    ::jxx::ext::xml::initialize();
}

} // namespace jxx::ext
