#include "util/logging/jxx.util.logging.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
#include "util/logging/jxx.util.logging.ErrorManager.h"
#include "util/logging/jxx.util.logging.Filter.h"
#include "util/logging/jxx.util.logging.Formatter.h"
#include "util/logging/jxx.util.logging.Handler.h"
#include "util/logging/jxx.util.logging.Logger.h"
#include "util/logging/jxx.util.logging.SimpleFormatter.h"
#include "util/logging/jxx.util.logging.Level.h"
#include "util/logging/jxx.util.logging.LogRecord.h"

namespace jxx::util::logging {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<ErrorManager>();
        registerType<Filter>();
        registerType<Formatter>();
        registerType<Handler>();
        registerType<Logger>();
        registerType<SimpleFormatter>();
        registerType<Level>();
        ::jxx::io::serialization::registerSerializableClass<Level>(
            static_cast<::jxx::lang::jlong>(-8176160795706313070LL),
            [] { return ::jxx::NEW<Level>(::jxx::NEW<::jxx::lang::String>(""), 0); });
        registerType<LogRecord>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::logging
