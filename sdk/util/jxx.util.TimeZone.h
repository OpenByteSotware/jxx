#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

class Date;

// Java-like TimeZone backed by TZif zoneinfo.
class TimeZone : public jxx::lang::Object {
public:
    virtual ~TimeZone() = default;

    virtual jxx::Ptr<jxx::lang::String> getID() const = 0;

    // Total offset (raw + dst) at instant, in milliseconds.
    virtual jxx::lang::jint getOffset(jxx::lang::jlong epochMillis) const = 0;

    // Abbreviation at instant (e.g., EST/EDT).
    virtual jxx::Ptr<jxx::lang::String> getAbbreviation(jxx::lang::jlong epochMillis) const = 0;

    virtual jxx::lang::jint getRawOffset() const = 0;
    virtual jxx::lang::jbool useDaylightTime() const = 0;
    virtual jxx::lang::jbool inDaylightTime(jxx::Ptr<Date> d) const = 0;
    static jxx::Ptr<TimeZone> getTimeZone(jxx::Ptr<jxx::lang::String> id);
    static jxx::Ptr<TimeZone> getDefault();
};

} // namespace jxx::util
