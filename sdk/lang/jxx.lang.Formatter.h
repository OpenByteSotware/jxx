#pragma once
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <ctime>
#include <locale>
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.FormatArg.h"

namespace jxx { namespace lang {

class Formatter : public Object {
public:
    Formatter() = default;
    ~Formatter() override = default;

    // Format into internal buffer and return a String
    String format(const String& fmt, const std::vector<FormatArg>& args) {
        out_.clear();
        const std::string f = fmt.toStdString();
        size_t nextIndex = 1; // 1-based
        for (size_t i = 0; i < f.size(); ++i) {
            if (f[i] != '%') { out_.push_back(f[i]); continue; }
            if (i + 1 < f.size() && f[i + 1] == '%') { out_.push_back('%'); ++i; continue; }
            if (i + 1 < f.size() && f[i + 1] == 'n') { out_.push_back('\n'); ++i; continue; }

            // Parse: [arg_index$][flags][width][.precision][t|T]conversion
            size_t j = i + 1;
            int argIndex = 0; bool hasIndex = false;
            while (j < f.size() && std::isdigit((unsigned char)f[j])) { argIndex = argIndex * 10 + (f[j] - '0'); ++j; }
            if (j < f.size() && f[j] == '$') { hasIndex = true; ++j; }
            bool left = false, plus = false, space = false, zero = false, comma = false, sharp = false, paren = false;
            for (;; ++j) {
                if (j >= f.size()) break;
                char fl = f[j]; bool any = true;
                switch (fl) {
                case '-': left = true; break; case '+': plus = true; break; case ' ': space = true; break;
                case '0': zero = true; break; case ',': comma = true; break; case '#': sharp = true; break; case '(': paren = true; break;
                default: any = false; break;
                }
                if (!any) break;
            }
            int width = 0; bool hasWidth = false; while (j < f.size() && std::isdigit((unsigned char)f[j])) { hasWidth = true; width = width * 10 + (f[j] - '0'); ++j; }
            int precision = -1; if (j < f.size() && f[j] == '.') { ++j; precision = 0; while (j < f.size() && std::isdigit((unsigned char)f[j])) { precision = precision * 10 + (f[j] - '0'); ++j; } }
            bool dateTime = false; if (j < f.size() && (f[j] == 't' || f[j] == 'T')) { dateTime = true; ++j; }
            if (j >= f.size()) break; char conv = f[j];

            auto getArg = [&](int idx)->FormatArg { if (idx<1 || (size_t)idx>args.size()) return FormatArg(""); return args[(size_t)idx - 1]; };
            FormatArg arg = hasIndex ? getArg(argIndex) : getArg((int)nextIndex++);

            auto S = [&](const std::string& s0) {
                std::string s = s0;
                if (comma) s = group_locale(s, locale_);
                if (hasWidth && (int)s.size() < width) {
                    if (left) s.append((size_t)(width - s.size()), ' ');
                    else if (zero) s.insert(0, (size_t)(width - s.size()), '0');
                    else s.insert(0, (size_t)(width - s.size()), ' ');
                }
                out_ += s;
                };

            // Date/time subset
            if (dateTime) {
                std::time_t tt = std::time(nullptr); const auto& v = arg.value();
                if (std::holds_alternative<long long>(v)) tt = (time_t)std::get<long long>(v);
                std::tm tm{};
#if defined(_WIN32)
                localtime_s(&tm, &tt);
#else
                localtime_r(&tt, &tm);
#endif
                char buf[128]; const char* fmtp = nullptr;
                switch (conv) {
                case 'H': fmtp = "%H"; break; case 'M': fmtp = "%M"; break; case 'S': fmtp = "%S"; break;
                case 'R': fmtp = "%H:%M"; break; case 'T': fmtp = "%H:%M:%S"; break;
                case 'Y': fmtp = "%Y"; break; case 'y': fmtp = "%y"; break;
                case 'm': fmtp = "%m"; break; case 'd': fmtp = "%d"; break;
                case 'b': fmtp = "%b"; break; case 'B': fmtp = "%B"; break;
                default: fmtp = "%c"; break;
                }
                std::strftime(buf, sizeof(buf), fmtp, &tm); S(buf); i = j; continue;
            }

            std::ostringstream oss; oss.imbue(locale_);
            if (precision >= 0) oss << std::setprecision(precision);
            if (hasWidth && !zero) oss << std::setw(width);
            if (left) oss << std::left; else oss << std::right;

            const auto& var = arg.value();

            switch (conv) {
            case 's': case 'S': {
                String sv = jxx::lang::toString(arg); auto t = sv.toStdString();
                if (conv == 'S') for (auto& ch : t) ch = (char)std::toupper((unsigned char)ch);
                S(t); break;
            }
            case 'b': case 'B': {
                std::string t = (std::holds_alternative<bool>(var) ? (std::get<bool>(var) ? "true" : "false") : "true");
                if (conv == 'B') for (auto& ch : t) ch = (char)std::toupper((unsigned char)ch);
                S(t); break;
            }
            case 'h': case 'H': {
                auto t = jxx::lang::toString(arg).toStdString(); std::size_t h = std::hash<std::string>{}(t);
                std::ostringstream tmp; tmp << std::hex << h; auto u = tmp.str(); if (conv == 'H') for (auto& ch : u) ch = (char)std::toupper((unsigned char)ch);
                S(u); break;
            }
            case 'c': case 'C': {
                char ch = 0; if (std::holds_alternative<long long>(var)) ch = (char)std::get<long long>(var); else if (std::holds_alternative<String>(var)) { auto t = std::get<String>(var).toStdString(); ch = t.empty() ? 0 : t[0]; }
                std::string t(1, ch); if (conv == 'C') t[0] = (char)std::toupper((unsigned char)t[0]); S(t); break;
            }
            case 'd': case 'o': case 'x': case 'X': {
                long long v = 0; if (std::holds_alternative<long long>(var)) v = std::get<long long>(var); else if (std::holds_alternative<double>(var)) v = (long long)std::llround(std::get<double>(var));
                if (paren && v < 0) { v = -v; oss << '('; }
                if (conv == 'd') oss << v;
                else if (conv == 'o') { oss << std::oct << v; }
                else if (conv == 'x' || conv == 'X') { oss << std::hex << (unsigned long long)v; }
                {
                    std::string t = oss.str(); if (conv == 'X') for (auto& ch : t) ch = (char)std::toupper((unsigned char)ch);
                    if (comma) t = group_locale(t, locale_);
                    if (conv == 'x' && sharp) t = "0x" + t; else if (conv == 'X' && sharp) t = "0X" + t;
                    if (paren && !t.empty() && t[0] == '(') t += ')';
                    S(t);
                }
                break;
            }
            case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A': {
                double v = 0.0; if (std::holds_alternative<double>(var)) v = std::get<double>(var); else if (std::holds_alternative<long long>(var)) v = (double)std::get<long long>(var);
                if (conv == 'e' || conv == 'E') oss << std::scientific << v; else if (conv == 'f') oss << std::fixed << v; else if (conv == 'a' || conv == 'A') { oss << std::hexfloat << v; }
                else oss << v;
                std::string t = oss.str(); if (conv == 'E' || conv == 'G' || conv == 'A') for (auto& ch : t) ch = (char)std::toupper((unsigned char)ch);
                if (comma) t = group_locale(t, locale_);
                if ((plus || space) && v >= 0 && (conv != 'a' && conv != 'A')) t = std::string(plus ? "+" : " ") + t;
                S(t); break;
            }
            case 'p': case 'P': { // percent
                double v = 0.0; if (std::holds_alternative<double>(var)) v = std::get<double>(var); else if (std::holds_alternative<long long>(var)) v = (double)std::get<long long>(var);
                double shown = sharp ? v : (v * 100.0);
                std::ostringstream nss; nss.imbue(locale_);
                if (precision >= 0) nss << std::fixed << std::setprecision(precision) << shown; else nss << shown;
                std::string t = nss.str(); if (comma) t = group_locale(t, locale_);
                if ((plus || space) && shown >= 0) t = std::string(plus ? "+" : " ") + t; t.push_back('%'); S(t); break;
            }
            case 'm': case 'M': { // currency
                double v = 0.0; if (std::holds_alternative<double>(var)) v = std::get<double>(var); else if (std::holds_alternative<long long>(var)) v = (double)std::get<long long>(var);
                const auto& mp = std::use_facet<std::moneypunct<char>>(locale_); int frac = (int)mp.frac_digits(); long double scale = 1.0L; for (int k = 0; k < frac; ++k) scale *= 10.0L; long long units = (long long)std::llround((long double)v * scale);
                std::ostringstream mss; mss.imbue(locale_); mss << std::showbase << std::put_money(units); S(mss.str()); break;
            }
            default: { out_.push_back('%'); out_.push_back(conv); break; }
            }
            i = j;
        }
        return String(out_);
    }

    const std::string& buffer() const noexcept { return out_; }
    void clear() { out_.clear(); }

    Formatter& setLocale(const std::string& locale_name) {
        try { locale_ = std::locale(locale_name.c_str()); }
        catch (...) { locale_ = std::locale(""); }
        return *this;
    }

    static std::string group_locale(const std::string& s, const std::locale& loc) {
        const auto& np = std::use_facet<std::numpunct<char>>(loc);
        char thousands = np.thousands_sep();
        std::string grouping = np.grouping();
        char decimal = np.decimal_point();
        std::string sign; std::string digits = s; std::string frac;
        if (!digits.empty() && (digits[0] == '-' || digits[0] == '+')) { sign.push_back(digits[0]); digits.erase(0, 1); }
        auto dot = digits.find('.');
        if (dot != std::string::npos) { frac = digits.substr(dot + 1); digits.erase(dot); }
        std::string out; out.reserve(digits.size() + digits.size() / 3 + 2);
        int idx = (int)digits.size() - 1; size_t gp_idx = 0; int cur_group = grouping.empty() ? 3 : (unsigned char)grouping[gp_idx];
        int count = 0;
        while (idx >= 0) {
            out.push_back(digits[(size_t)idx]);
            ++count; --idx;
            if (cur_group > 0 && count == cur_group && idx >= 0) {
                out.push_back(thousands);
                count = 0;
                if (gp_idx + 1 < grouping.size() && grouping[gp_idx + 1] > 0) { ++gp_idx; cur_group = (unsigned char)grouping[gp_idx]; }
            }
        }
        std::reverse(out.begin(), out.end());
        if (!frac.empty()) { out.push_back(decimal); out += frac; }
        return sign + out;
    }

private:
    std::string out_;
    std::locale locale_ = std::locale("");
};

}} // namespace jxx::lang
