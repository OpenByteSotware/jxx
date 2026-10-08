#pragma once

#include "text/jxx.text.Format.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"


namespace jxx::lang {
class String;
}

namespace jxx::util { class Locale; }
namespace jxx::util { class TimeZone; }

namespace jxx::text {

class DateFormat : public Format {
public:
    using JxxSuper = Format;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<DateFormat, JxxSuper>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

private:
    ::jxx::Ptr<::jxx::lang::String> pattern_;
    ::jxx::Ptr<::jxx::util::Locale> locale_;

public:
    DateFormat(const ::jxx::Ptr<::jxx::lang::String>& pattern, const ::jxx::Ptr<::jxx::util::Locale>& locale);

    static ::jxx::Ptr<DateFormat> ofPattern(const ::jxx::Ptr<::jxx::lang::String>& pattern,
                                          const ::jxx::Ptr<::jxx::util::Locale>& locale);

    ::jxx::Ptr<::jxx::lang::String> format(::jxx::lang::jlong epochMillis,
                                       const ::jxx::Ptr<::jxx::util::TimeZone>& timeZone) const;
};

} // namespace jxx::text
