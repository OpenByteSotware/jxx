#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"


namespace jxx::lang {
class String;
}

namespace jxx::util {
    class Locale;

class NumberFormat : public ::jxx::lang::Object {
public:
    using JxxSuper = ::jxx::lang::Object;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<NumberFormat, JxxSuper>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

protected:
    ::jxx::Ptr<Locale> locale_;
    ::jxx::lang::jchar decimalSeparator_;
    ::jxx::lang::jchar groupingSeparator_;
    ::jxx::lang::jbool groupingUsed_;

public:
    NumberFormat();
    virtual ~NumberFormat() = default;

    static ::jxx::Ptr<NumberFormat> getInstance(const ::jxx::Ptr<Locale>& locale);

    virtual ::jxx::Ptr<::jxx::lang::String> format(::jxx::lang::jlong value) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> format(::jxx::lang::jdouble value) = 0;

    void setGroupingUsed(::jxx::lang::jbool groupingUsed);
    ::jxx::lang::jbool isGroupingUsed() const;
    char getDecimalSeparator() const;
    char getGroupingSeparator() const;
};

} // namespace jxx::util

