#include "jxx.initialize.h"

#include <mutex>

#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.initialize.h"
#include "io/jxx.io.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.h"
#include "math/jxx.math.ClassInitialization.h"
#include "net/jxx.net.ClassInitialization.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "security/jxx.security.ClassInitialization.h"

namespace jxx {

void initialize()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
            jxx::lang::initialize();
            jxx::lang::initializeClasses();
            jxx::io::initializeClasses();
            jxx::util::initializeClasses();
            jxx::math::initializeClasses();
            jxx::net::initializeClasses();
            jxx::nio::initializeClasses();
            jxx::security::initializeClasses();
        });
}

} // namespace jxx
