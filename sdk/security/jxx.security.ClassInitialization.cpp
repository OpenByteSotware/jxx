#include "security/jxx.security.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::security {

namespace {

template <typename T>
void registerType()
{
    jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
        });
}

} // namespace jxx::security
