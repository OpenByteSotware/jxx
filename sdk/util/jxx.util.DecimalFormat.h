#pragma once

#include "util/jxx.util.NumberFormat.h"

namespace jxx::util {

class DecimalFormat : public NumberFormat {
private:
    jxx::lang::jint minFractionDigits_;
    jxx::lang::jint maxFractionDigits_;

public:
    DecimalFormat();

    static jxx::Ptr<DecimalFormat> ofPattern(const jxx::Ptr<jxx::lang::String>& pattern,
                                             const jxx::Ptr<Locale>& locale);

    jxx::Ptr<jxx::lang::String> format(jxx::lang::jlong value) override;
    jxx::Ptr<jxx::lang::String> format(jxx::lang::jdouble value) override;

    void setMinimumFractionDigits(jxx::lang::jint value);
    void setMaximumFractionDigits(jxx::lang::jint value);
};

} // namespace jxx::util
