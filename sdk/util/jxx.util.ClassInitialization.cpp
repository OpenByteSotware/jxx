#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.Detail.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/function/jxx.util.function.ClassInitialization.h"
#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/regex/jxx.util.regex.ClassInitialization.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"
#include "util/stream/jxx.util.stream.ClassInitialization.h"

#include <mutex>

namespace jxx::util {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        class_initialization_detail::initializeExceptions();
        class_initialization_detail::initializeCollections();
        class_initialization_detail::initializeMaps();
        class_initialization_detail::initializeDateTime();
        class_initialization_detail::initializeLocaleResources();
        class_initialization_detail::initializeUtilities();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::util::concurrent::initialize();
    ::jxx::util::function::initialize();
    ::jxx::util::logging::initialize();
    ::jxx::util::regex::initialize();
    ::jxx::util::stream::initialize();
    ::jxx::util::zip::initialize();
}

}
