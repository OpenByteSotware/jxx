#include "jxx.initialize.h"

#include <mutex>

#include "com/google/gson/jxx.com.google.gson.ClassInitialization.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.ClassInitialization.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ClassInitialization.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"
#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"
#include "io/jxx.io.ClassInitialization.h"
#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.initialize.h"
#include "lang/jxx.lang.System.h"
#include "math/jxx.math.ClassInitialization.h"
#include "net/jxx.net.ClassInitialization.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "nio/channels/jxx.nio.channels.ClassInitialization.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ClassInitialization.h"
#include "org/xml/sax/jxx.org.xml.sax.ClassInitialization.h"
#include "security/jxx.security.ClassInitialization.h"
#include "text/jxx.text.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/function/jxx.util.function.ClassInitialization.h"
#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/regex/jxx.util.regex.ClassInitialization.h"

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
        ::jxx::util::function::initializeClasses();
        ::jxx::util::logging::initializeClasses();
        ::jxx::util::regex::initializeClasses();
        ::jxx::math::initializeClasses();
        ::jxx::net::initializeClasses();
        ::jxx::nio::initializeClasses();
        ::jxx::nio::channels::initializeClasses();
        ::jxx::security::initializeClasses();
        ::jxx::text::initializeClasses();
        ::jxx::org::w3c::dom::initializeClasses();
        ::jxx::org::xml::sax::initializeClasses();
        ::com::google::gson::initializeClasses();
        ::jxx::com::sun::net::httpserver::initializeClasses();
        ::jxx::ext::net::ssl::initializeClasses();
        ::jxx::ext::xml::validation::initializeClasses();
        ::jxx::ext::xml::parsers::initializeClasses();
    });
}

} // namespace jxx
