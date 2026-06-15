#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class ZonedDateTime : public jxx::lang::Object {
private:
    jxx::lang::jlong epochMillis_;
    jxx::Ptr<TimeZone> timeZone_;

public:
    ZonedDateTime(jxx::lang::jlong epochMillis, const jxx::Ptr<TimeZone>& timeZone);
    static jxx::Ptr<ZonedDateTime> ofEpochMillis(jxx::lang::jlong epochMillis,
                                                 const jxx::Ptr<TimeZone>& timeZone);
    static jxx::Ptr<ZonedDateTime> now(const jxx::Ptr<TimeZone>& timeZone);

    jxx::lang::jlong toEpochMillis() const;
    jxx::Ptr<TimeZone> getTimeZone() const;
    jxx::Ptr<jxx::lang::String> toString() const override;
};

} // namespace jxx::util
