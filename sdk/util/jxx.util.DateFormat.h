#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"


namespace jxx::lang {
class String;
}

namespace jxx::util {
    class Locale;
	class TimeZone;

class DateFormat : public jxx::lang::Object {
private:
    jxx::Ptr<jxx::lang::String> pattern_;
    jxx::Ptr<Locale> locale_;

public:
    DateFormat(const jxx::Ptr<jxx::lang::String>& pattern, const jxx::Ptr<Locale>& locale);

    static jxx::Ptr<DateFormat> ofPattern(const jxx::Ptr<jxx::lang::String>& pattern,
                                          const jxx::Ptr<Locale>& locale);

    jxx::Ptr<jxx::lang::String> format(jxx::lang::jlong epochMillis,
                                       const jxx::Ptr<TimeZone>& timeZone) const;
};

} // namespace jxx::util
