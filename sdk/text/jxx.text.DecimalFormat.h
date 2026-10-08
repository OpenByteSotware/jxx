#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "text/jxx.text.NumberFormat.h"

namespace jxx::text {

class DecimalFormat : public NumberFormat {
private:
    jxx::lang::jint minFractionDigits_;
    jxx::lang::jint maxFractionDigits_;

public:
    using Super = NumberFormat;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<DecimalFormat, NumberFormat>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    DecimalFormat();

    static jxx::Ptr<DecimalFormat> ofPattern(const jxx::Ptr<jxx::lang::String>& pattern,
                                             const jxx::Ptr<::jxx::util::Locale>& locale);

    jxx::Ptr<jxx::lang::String> format(jxx::lang::jlong value) override;
    jxx::Ptr<jxx::lang::String> format(jxx::lang::jdouble value) override;

    void setMinimumFractionDigits(jxx::lang::jint value);
    void setMaximumFractionDigits(jxx::lang::jint value);
};

} // namespace jxx::text
