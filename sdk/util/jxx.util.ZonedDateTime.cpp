#include <chrono>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.DateFormat.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.StringPool.h"
#include "util/jxx.util.ZonedDateTime.h"

namespace jxx::util {

ZonedDateTime::ZonedDateTime(jxx::lang::jlong epochMillis, const jxx::Ptr<TimeZone>& timeZone)
    : epochMillis_(epochMillis), timeZone_(timeZone) {}

jxx::Ptr<ZonedDateTime> ZonedDateTime::ofEpochMillis(jxx::lang::jlong epochMillis,
                                                     const jxx::Ptr<TimeZone>& timeZone) {
    return jxx::NEW<ZonedDateTime>(epochMillis, timeZone);
}

jxx::Ptr<ZonedDateTime> ZonedDateTime::now(const jxx::Ptr<TimeZone>& timeZone) {
    using namespace std::chrono;
    jxx::lang::jlong epochMillis = static_cast<jxx::lang::jlong>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    return ofEpochMillis(epochMillis, timeZone);
}

jxx::lang::jlong ZonedDateTime::toEpochMillis() const { return epochMillis_; }
jxx::Ptr<TimeZone> ZonedDateTime::getTimeZone() const { return timeZone_; }

jxx::Ptr<jxx::lang::String> ZonedDateTime::toString() const {
    auto fmt = DateFormat::ofPattern(StringPool::intern("yyyy-MM-dd HH:mm:ss z"), Locale::getDefault());
    return fmt->format(epochMillis_, timeZone_);
}

} // namespace jxx::util
