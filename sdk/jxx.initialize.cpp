#include "jxx.initialize.h"
#include "jxx.BuildConfig.h"

#if !JXX_HEADLESS
#include "jxx.gui.initialize.h"
#endif

#include "lang/jxx.lang.initialize.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.ClassInitialization.h"
#include "awt/jxx.awt.ClassInitialization.h"
#include "beans/jxx.beans.ClassInitialization.h"
#include "com/jxx.com.ClassInitialization.h"
#include "ext/jxx.ext.ClassInitialization.h"
#include "io/jxx.io.ClassInitialization.h"
#include "math/jxx.math.ClassInitialization.h"
#include "net/jxx.net.ClassInitialization.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "org/jxx.org.ClassInitialization.h"
#include "security/jxx.security.ClassInitialization.h"
#include "text/jxx.text.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.h"

#include <mutex>
#include <unordered_set>

namespace {
std::mutex& clientInitializerMutex() {
    static std::mutex mutex;
    return mutex;
}
std::unordered_set<jxx::ClientInitializer>& completedClientInitializers() {
    static std::unordered_set<jxx::ClientInitializer> values;
    return values;
}
std::unordered_set<jxx::ClientInitializer>& runningClientInitializers() {
    static std::unordered_set<jxx::ClientInitializer> values;
    return values;
}
} // namespace

namespace jxx {

void initialize() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        ::jxx::lang::System::init();
        ::jxx::lang::initialize();
        ::jxx::lang::initializeClasses();
        ::jxx::awt::initialize();
        ::jxx::beans::initialize();
        ::com::initialize();
        ::jxx::ext::initialize();
        ::jxx::io::initialize();
        ::jxx::math::initialize();
        ::jxx::net::initialize();
        ::jxx::nio::initialize();
        ::jxx::org::initialize();
        ::jxx::security::initialize();
        ::jxx::text::initialize();
        ::jxx::util::initialize();
#if !JXX_HEADLESS
        ::jxx::gui::initialize();
#endif
    });
}



void initialize(ClientInitializer callback) {
    initialize();
    if (callback == nullptr) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(clientInitializerMutex());
        if (completedClientInitializers().count(callback) != 0U ||
            runningClientInitializers().count(callback) != 0U) {
            return;
        }
        runningClientInitializers().insert(callback);
    }
    try {
        callback();
        std::lock_guard<std::mutex> lock(clientInitializerMutex());
        runningClientInitializers().erase(callback);
        completedClientInitializers().insert(callback);
    } catch (...) {
        std::lock_guard<std::mutex> lock(clientInitializerMutex());
        runningClientInitializers().erase(callback);
        throw;
    }
}

} // namespace jxx
