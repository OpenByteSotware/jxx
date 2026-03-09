#pragma once
#include "jxx.lang.String.h"
#include <variant>
#include <cstdio>

namespace jxx { namespace lang {

class FormatArg {
public:
    using Var = std::variant<String, long long, double, bool>;
    FormatArg(const String& s) : v_(s) {}
    FormatArg(const char* s) : v_(String(s)) {}
    FormatArg(long long i) : v_(i) {}
    FormatArg(int i) : v_(static_cast<long long>(i)) {}
    FormatArg(double d) : v_(d) {}
    FormatArg(float f) : v_(static_cast<double>(f)) {}
    FormatArg(bool b) : v_(b) {}

    const Var& value() const { return v_; }
private:
    Var v_;
};

inline String toString(const FormatArg& a, int precision = -1) {
    const auto& v = a.value();
    if (std::holds_alternative<String>(v)) return std::get<String>(v);
    if (std::holds_alternative<long long>(v)) return String(std::to_string(std::get<long long>(v)));
    if (std::holds_alternative<double>(v)) {
        double d = std::get<double>(v);
        if (precision >= 0) {
            char buf[128];
            std::snprintf(buf, sizeof(buf), ("% ." + std::to_string(precision) + "f").c_str(), d);
            return String(buf);
        }
        return String(std::to_string(d));
    }
    if (std::holds_alternative<bool>(v)) return String(std::get<bool>(v) ? "true" : "false");
    return String("");
}

}} // namespace jxx::lang
