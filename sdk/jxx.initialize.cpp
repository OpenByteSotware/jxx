#include "jxx.initialize.h"

#include <mutex>

#include "com/google/gson/jxx.com.google.gson.ClassInitialization.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.ClassInitialization.h"
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.ClassInitialization.h"
#include "ext/crypto/jxx.ext.crypto.ClassInitialization.h"
#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.ClassInitialization.h"
#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"
#include "ext/security/cert/internal/jxx.ext.security.cert.internal.ClassInitialization.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ClassInitialization.h"
#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.ClassInitialization.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"
#include "ext/xml/transform/jxx.ext.xml.transform.ClassInitialization.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.ClassInitialization.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.ClassInitialization.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.ClassInitialization.h"
#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"
#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.ClassInitialization.h"
#include "io/jxx.io.ClassInitialization.h"
#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.initialize.h"
#include "lang/jxx.lang.System.h"
#include "math/jxx.math.ClassInitialization.h"
#include "net/jxx.net.ClassInitialization.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"
#include "nio/channels/jxx.nio.channels.ClassInitialization.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ClassInitialization.h"
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.ClassInitialization.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.ClassInitialization.h"
#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.ClassInitialization.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.ClassInitialization.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.ClassInitialization.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.ClassInitialization.h"
#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.ClassInitialization.h"
#include "org/xml/sax/jxx.org.xml.sax.ClassInitialization.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.ClassInitialization.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ClassInitialization.h"
#include "org/xml/sax/internal/jxx.org.xml.sax.internal.ClassInitialization.h"
#include "security/jxx.security.ClassInitialization.h"
#include "text/jxx.text.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"
#include "util/function/jxx.util.function.ClassInitialization.h"
#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/regex/jxx.util.regex.ClassInitialization.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"

namespace jxx {

void initialize() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        ::jxx::lang::System::init();
        ::jxx::lang::initialize();
        ::jxx::lang::initializeClasses();
        ::jxx::io::initializeClasses();
        ::jxx::util::initializeClasses();
        ::jxx::util::concurrent::initializeClasses();
        ::jxx::util::concurrent::locks::initializeClasses();
        ::jxx::util::function::initializeClasses();
        ::jxx::util::logging::initializeClasses();
        ::jxx::util::regex::initializeClasses();
        ::jxx::util::zip::initializeClasses();
        ::jxx::math::initializeClasses();
        ::jxx::net::initializeClasses();
        ::jxx::nio::initializeClasses();
        ::jxx::nio::file::attribute::initializeClasses();
        ::jxx::nio::channels::initializeClasses();
        ::jxx::security::initializeClasses();
        ::jxx::text::initializeClasses();
        ::jxx::org::w3c::dom::initializeClasses();
        ::jxx::org::w3c::dom::events::internal::initializeClasses();
        ::jxx::org::w3c::dom::internal::initializeClasses();
        ::jxx::org::w3c::dom::bootstrap::initializeClasses();
        ::jxx::org::w3c::dom::ls::initializeClasses();
        ::jxx::org::w3c::dom::traversal::initializeClasses();
        ::jxx::org::w3c::dom::ls::internal::initializeClasses();
        ::jxx::org::w3c::dom::views::internal::initializeClasses();
        ::jxx::org::xml::sax::initializeClasses();
        ::jxx::org::xml::sax::ext::initializeClasses();
        ::jxx::org::xml::sax::helpers::initializeClasses();
        ::jxx::org::xml::sax::internal::initializeClasses();
        ::com::google::gson::initializeClasses();
        ::jxx::com::sun::net::httpserver::initializeClasses();
        ::jxx::com::sun::net::httpserver::spi::initializeClasses();
        ::jxx::ext::security::auth::initializeClasses();
        ::jxx::ext::security::auth::x500::initializeClasses();
        ::jxx::ext::security::cert::initializeClasses();
        ::jxx::ext::security::cert::internal::initializeClasses();
        ::jxx::ext::crypto::initializeClasses();
        ::jxx::ext::net::ssl::initializeClasses();
        ::jxx::ext::xml::bind::initializeClasses();
        ::jxx::ext::xml::namespace_::initializeClasses();
        ::jxx::ext::xml::transform::initializeClasses();
        ::jxx::ext::xml::transform::dom::initializeClasses();
        ::jxx::ext::xml::transform::internal::initializeClasses();
        ::jxx::ext::xml::transform::stream::initializeClasses();
        ::jxx::ext::xml::xpath::initializeClasses();
        ::jxx::ext::xml::xpath::internal::initializeClasses();
        ::jxx::ext::xml::validation::initializeClasses();
        ::jxx::ext::xml::parsers::initializeClasses();
    });
}

} // namespace jxx
