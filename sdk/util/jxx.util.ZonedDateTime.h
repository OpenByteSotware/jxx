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
    jlong epochMillis_;
    jxx::Ptr<TimeZone> timeZone_;

public:
    ZonedDateTime(jlong epochMillis, const jxx::Ptr<TimeZone>& timeZone);

    static jxx::Ptr<ZonedDateTime> ofEpochMillis(jlong epochMillis,
                                                 const jxx::Ptr<TimeZone>& timeZone);
    static jxx::Ptr<ZonedDateTime> now(const jxx::Ptr<TimeZone>& timeZone);

    jlong toEpochMillis() const;
    jxx::Ptr<TimeZone> getTimeZone() const;
    jxx::Ptr<jxx::lang::String> toString() const override;
};

} // namespace jxx::util
