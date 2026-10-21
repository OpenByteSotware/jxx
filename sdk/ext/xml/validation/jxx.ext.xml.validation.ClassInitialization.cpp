#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"

#include <mutex>

#include "ext/xml/validation/jxx.ext.xml.validation.Schema.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::validation {
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
        registerType<Schema>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::validation
