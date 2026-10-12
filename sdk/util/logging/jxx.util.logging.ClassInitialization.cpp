#include "util/logging/jxx.util.logging.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
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
        registerType<Level>();
        registerType<LogRecord>();
    });
}

} // namespace jxx::util::logging
