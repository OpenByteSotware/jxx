#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.Locale.h"

namespace jxx::util {

struct FormatSpec {
    jxx::lang::jint argIndex = -1;
    jxx::lang::jbool reuseLast = false;

    jxx::lang::jbool leftJustify = false;
    jxx::lang::jbool alt = false;
    jxx::lang::jbool plus = false;
    jxx::lang::jbool leadingSpace = false;
    jxx::lang::jbool zeroPad = false;
    jxx::lang::jbool group = false;
    jxx::lang::jbool parens = false;
    jxx::lang::jint width = -1;
    jxx::lang::jint precision = -1;

    jxx::lang::jbool dateTime = false;
    jxx::lang::jbool upper = false;
    jxx::lang::jchar conv = 0;
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
