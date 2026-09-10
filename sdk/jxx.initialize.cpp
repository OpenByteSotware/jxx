#include "jxx.initialize.h"

#include <mutex>

#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"
#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"
#include "io/jxx.io.ClassInitialization.h"
#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.initialize.h"
#include "math/jxx.math.ClassInitialization.h"
#include "net/jxx.net.ClassInitialization.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ClassInitialization.h"
#include "org/xml/sax/jxx.org.xml.sax.ClassInitialization.h"
#include "security/jxx.security.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.h"

namespace jxx {

void initialize() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        ::jxx::lang::initialize();
        ::jxx::lang::initializeClasses();
        ::jxx::io::initializeClasses();
        ::jxx::util::initializeClasses();
        ::jxx::math::initializeClasses();
        ::jxx::net::initializeClasses();
        ::jxx::nio::initializeClasses();
        ::jxx::security::initializeClasses();
        ::jxx::org::w3c::dom::initializeClasses();
        ::jxx::org::xml::sax::initializeClasses();
        ::jxx::ext::xml::validation::initializeClasses();
        ::jxx::ext::xml::parsers::initializeClasses();
    });
}

} // namespace jxx
