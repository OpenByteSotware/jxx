#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util { class Locale; }

namespace jxx::text {

class DateFormatSymbols final
    : public ::jxx::lang::ClassBase<
          DateFormatSymbols, ::jxx::lang::Object,
          ::jxx::lang::Cloneable, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DateFormatSymbols, JxxSuper,
        ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    DateFormatSymbols();
    explicit DateFormatSymbols(const ::jxx::Ptr<::jxx::util::Locale>& locale);

    static ::jxx::Ptr<DateFormatSymbols> getInstance();
    static ::jxx::Ptr<DateFormatSymbols> getInstance(
        const ::jxx::Ptr<::jxx::util::Locale>& locale);

    ::jxx::Ptr<StringArray> getEras() const;
    void setEras(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getMonths() const;
    void setMonths(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getShortMonths() const;
    void setShortMonths(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getWeekdays() const;
    void setWeekdays(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getShortWeekdays() const;
    void setShortWeekdays(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> getAmPmStrings() const;
    void setAmPmStrings(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<::jxx::lang::String> getLocalPatternChars() const;
    void setLocalPatternChars(const ::jxx::Ptr<::jxx::lang::String>& value);

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override {}
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override {}
    void readObjectNoData() override {}

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    static ::jxx::Ptr<StringArray> copy_(const ::jxx::Ptr<StringArray>& values);
    ::jxx::Ptr<StringArray> eras_;
    ::jxx::Ptr<StringArray> months_;
    ::jxx::Ptr<StringArray> shortMonths_;
    ::jxx::Ptr<StringArray> weekdays_;
    ::jxx::Ptr<StringArray> shortWeekdays_;
    ::jxx::Ptr<StringArray> amPmStrings_;
    ::jxx::Ptr<::jxx::lang::String> localPatternChars_;
};

} // namespace jxx::text
