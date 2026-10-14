#pragma once

#include "text/jxx.text.DateFormat.h"

#include <string>

namespace jxx::lang { class String; }
namespace jxx::util { class Date; class Locale; class TimeZone; }

namespace jxx::text {

class ParsePosition;
class DateFormatSymbols;

class SimpleDateFormat final : public DateFormat {
public:
    using JxxSuper = DateFormat;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<SimpleDateFormat, JxxSuper>;

    SimpleDateFormat();
    explicit SimpleDateFormat(
        const ::jxx::Ptr<::jxx::lang::String>& pattern);
    SimpleDateFormat(
        const ::jxx::Ptr<::jxx::lang::String>& pattern,
        const ::jxx::Ptr<::jxx::util::Locale>& locale);
    SimpleDateFormat(
        const ::jxx::Ptr<::jxx::lang::String>& pattern,
        const ::jxx::Ptr<DateFormatSymbols>& symbols);
    ::jxx::Ptr<DateFormatSymbols> getDateFormatSymbols() const;
    void setDateFormatSymbols(const ::jxx::Ptr<DateFormatSymbols>& symbols);

    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::util::Date>& date) const;
    ::jxx::Ptr<::jxx::lang::String> format(
        ::jxx::lang::jlong epochMillis) const;

    ::jxx::Ptr<::jxx::util::Date> parse(
        const ::jxx::Ptr<::jxx::lang::String>& source) const;
    ::jxx::Ptr<::jxx::util::Date> parse(
        const ::jxx::Ptr<::jxx::lang::String>& source,
        const ::jxx::Ptr<ParsePosition>& position) const;

    void applyPattern(const ::jxx::Ptr<::jxx::lang::String>& pattern);
    void applyLocalizedPattern(
        const ::jxx::Ptr<::jxx::lang::String>& pattern);
    ::jxx::Ptr<::jxx::lang::String> toPattern() const;
    ::jxx::Ptr<::jxx::lang::String> toLocalizedPattern() const;

    void setTimeZone(const ::jxx::Ptr<::jxx::util::TimeZone>& timeZone);
    ::jxx::Ptr<::jxx::util::TimeZone> getTimeZone() const;

    void setLenient(::jxx::lang::jbool lenient) noexcept;
    ::jxx::lang::jbool isLenient() const noexcept;

    void set2DigitYearStart(const ::jxx::Ptr<::jxx::util::Date>& startDate);
    ::jxx::Ptr<::jxx::util::Date> get2DigitYearStart() const;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    void validatePattern_(const std::string& pattern) const;
    ::jxx::Ptr<::jxx::lang::String> formatImpl_(
        ::jxx::lang::jlong epochMillis) const;

    ::jxx::Ptr<::jxx::lang::String> pattern_;
    ::jxx::Ptr<::jxx::util::Locale> locale_;
    ::jxx::Ptr<::jxx::util::TimeZone> timeZone_;
    ::jxx::Ptr<::jxx::util::Date> twoDigitYearStart_;
    ::jxx::Ptr<DateFormatSymbols> symbols_;
    ::jxx::lang::jbool lenient_ = true;
};

} // namespace jxx::text
