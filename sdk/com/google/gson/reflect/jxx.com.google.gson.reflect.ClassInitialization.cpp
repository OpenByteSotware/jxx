#include "com/google/gson/reflect/jxx.com.google.gson.reflect.ClassInitialization.h"

#include <mutex>

#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"

namespace com::google::gson::reflect {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            TypeToken,
            ::jxx::lang::Object>::Class();
    });
}

} // namespace com::google::gson::reflect
