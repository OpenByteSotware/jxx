#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.Locale.h"

namespace jxx::util {

struct FormatSpec {
    jint argIndex = -1;
    jbool reuseLast = false;

    jbool leftJustify = false;
    jbool alt = false;
    jbool plus = false;
    jbool leadingSpace = false;
    jbool zeroPad = false;
    jbool group = false;
    jbool parens = false;

    jint width = -1;
    jint precision = -1;

    jbool dateTime = false;
    jbool upper = false;
    char conv = 0;
};

class Formatter final {
public:
    explicit Formatter(jxx::Ptr<jxx::lang::Locale> locale = nullptr);

    jxx::Ptr<jxx::lang::String> format(
        jxx::Ptr<jxx::lang::String> fmt,
        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args);

private:
    jxx::Ptr<jxx::lang::Locale> locale_;
};

} // namespace jxx::util
