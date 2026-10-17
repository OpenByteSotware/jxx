#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.SimpleTimeZone.h"
#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.ZonedDateTime.h"

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeDateTime() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Calendar>();
        ::jxx::io::serialization::registerSerializableClass<Calendar>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                    Calendar, 1U, 2U>();
        registerType<Date>();
        ::jxx::io::serialization::registerSerializableClass<Date>(1);
        registerType<GregorianCalendar>();
        ::jxx::io::serialization::registerSerializableClass<GregorianCalendar>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<GregorianCalendar, 1U, 2U>();
        registerType<SimpleTimeZone>();
        registerType<TimeZone>();
        registerType<ZonedDateTime>();
    });
}
}
