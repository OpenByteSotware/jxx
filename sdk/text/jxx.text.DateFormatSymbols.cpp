#include "text/jxx.text.DateFormatSymbols.h"

#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Locale.h"

namespace jxx::text {
namespace {
using String = ::jxx::lang::String;
using Array = DateFormatSymbols::StringArray;
::jxx::Ptr<Array> strings(std::initializer_list<const char*> values) {
    std::vector<::jxx::Ptr<String>> result;
    for (const auto* value : values) result.push_back(::jxx::NEW<String>(value));
    return ::jxx::NEW<Array>(std::move(result));
}
bool arrayEquals(const ::jxx::Ptr<Array>& left, const ::jxx::Ptr<Array>& right) {
    if (left == nullptr || right == nullptr || left->length != right->length) return false;
    for (::jxx::lang::jint index = 0; index < left->length; ++index) {
        if (!(*left)[index]->equals((*right)[index])) return false;
    }
    return true;
}
} // namespace

DateFormatSymbols::DateFormatSymbols()
    : DateFormatSymbols(::jxx::util::Locale::getDefault()) {}

DateFormatSymbols::DateFormatSymbols(const ::jxx::Ptr<::jxx::util::Locale>& locale) {
    if (locale == nullptr) throw ::jxx::lang::NullPointerException();
    eras_ = strings({"BC", "AD"});
    months_ = strings({"January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December", ""});
    shortMonths_ = strings({"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", ""});
    weekdays_ = strings({"", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"});
    shortWeekdays_ = strings({"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"});
    amPmStrings_ = strings({"AM", "PM"});
    localPatternChars_ = ::jxx::NEW<String>("GyMdkHmsSEDFwWahKzZYuXL");
}

::jxx::Ptr<DateFormatSymbols> DateFormatSymbols::getInstance() {
    return ::jxx::NEW<DateFormatSymbols>();
}
::jxx::Ptr<DateFormatSymbols::LocaleArray> DateFormatSymbols::getAvailableLocales() {
    std::vector<::jxx::Ptr<::jxx::util::Locale>> values{
        ::jxx::util::Locale::ROOT, ::jxx::util::Locale::US,
        ::jxx::util::Locale::UK, ::jxx::util::Locale::CANADA,
        ::jxx::util::Locale::FRANCE, ::jxx::util::Locale::GERMANY,
        ::jxx::util::Locale::JAPAN, ::jxx::util::Locale::KOREA};
    return ::jxx::NEW<LocaleArray>(std::move(values));
}
::jxx::Ptr<DateFormatSymbols> DateFormatSymbols::getInstance(
    const ::jxx::Ptr<::jxx::util::Locale>& locale) {
    return ::jxx::NEW<DateFormatSymbols>(locale);
}
::jxx::Ptr<Array> DateFormatSymbols::copy_(const ::jxx::Ptr<Array>& values) {
    if (values == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::NEW<Array>(*values);
}
#define JXX_SYMBOL_ACCESSORS(Name, field) \
::jxx::Ptr<Array> DateFormatSymbols::get##Name() const { return copy_(field); } \
void DateFormatSymbols::set##Name(const ::jxx::Ptr<Array>& values) { field = copy_(values); }
JXX_SYMBOL_ACCESSORS(Eras, eras_)
JXX_SYMBOL_ACCESSORS(Months, months_)
JXX_SYMBOL_ACCESSORS(ShortMonths, shortMonths_)
JXX_SYMBOL_ACCESSORS(Weekdays, weekdays_)
JXX_SYMBOL_ACCESSORS(ShortWeekdays, shortWeekdays_)
JXX_SYMBOL_ACCESSORS(AmPmStrings, amPmStrings_)
#undef JXX_SYMBOL_ACCESSORS
::jxx::Ptr<String> DateFormatSymbols::getLocalPatternChars() const { return localPatternChars_; }
void DateFormatSymbols::setLocalPatternChars(const ::jxx::Ptr<String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    localPatternChars_ = value;
}
::jxx::lang::jbool DateFormatSymbols::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto value = ::jxx::CAST<DateFormatSymbols>(other);
    return value != nullptr && arrayEquals(eras_, value->eras_) &&
        arrayEquals(months_, value->months_) && arrayEquals(shortMonths_, value->shortMonths_) &&
        arrayEquals(weekdays_, value->weekdays_) && arrayEquals(shortWeekdays_, value->shortWeekdays_) &&
        arrayEquals(amPmStrings_, value->amPmStrings_) && localPatternChars_->equals(value->localPatternChars_);
}
::jxx::lang::jint DateFormatSymbols::hashCode() const { return localPatternChars_->hashCode(); }
::jxx::Ptr<::jxx::lang::Object> DateFormatSymbols::cloneImpl() const {
    auto copy = ::jxx::NEW<DateFormatSymbols>();
    copy->eras_ = copy_(eras_); copy->months_ = copy_(months_); copy->shortMonths_ = copy_(shortMonths_);
    copy->weekdays_ = copy_(weekdays_); copy->shortWeekdays_ = copy_(shortWeekdays_);
    copy->amPmStrings_ = copy_(amPmStrings_); copy->localPatternChars_ = localPatternChars_;
    return ::jxx::CAST<::jxx::lang::Object>(copy);
}
} // namespace jxx::text
