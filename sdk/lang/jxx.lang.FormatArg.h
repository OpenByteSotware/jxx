#pragma once
#include "lang/jxx.lang.String.h"
#include <variant>

namespace jxx { namespace lang {

class FormatArg {
public:
    using Var = std::variant<String, long long, double, bool>;
    FormatArg(const String& s);
    FormatArg(const char* s);
    FormatArg(long long i);
    FormatArg(int i);
    FormatArg(double d);
    FormatArg(float f);
    FormatArg(bool b);

    const Var& value() const;
private:
    Var v_;
};

String toString(const FormatArg& a, int precision = -1);

}} // namespace jxx::lang
