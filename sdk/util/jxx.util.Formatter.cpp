#include "jxx.util.Formatter.h"

#include "jxx.util.format.IllegalFormatException.h"
#include "jxx.util.format.MissingFormatArgumentException.h"
#include "jxx.util.format.UnknownFormatConversionException.h"

#include "jxx.util.Date.h"
#include "jxx.util.Calendar.h" // Calender alias
#include "jxx.util.TimeZone.h"

#include "jxx.lang.Integer.h"
#include "jxx.lang.Long.h"
#include "jxx.lang.Short.h"
#include "jxx.lang.Byte.h"
#include "jxx.lang.Boolean.h"
#include "jxx.lang.Float.h"
#include "jxx.lang.Double.h"
#include "jxx.lang.Character.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

namespace jxx::util {

static inline void throw_fmt(const char* msg) {
    throw jxx::util::format::IllegalFormatException(std::make_shared<jxx::lang::String>(msg));
}

static inline bool is_digit(char c){ return c>='0' && c<='9'; }

static inline std::string up(std::string s){ for(auto& c:s) c=(char)std::toupper((unsigned char)c); return s; }

static inline bool has_long(jxx::Ptr<jxx::lang::Object> o, long long& out) {
    if (auto i = std::dynamic_pointer_cast<jxx::lang::Integer>(o)) { out = (long long)i->intValue(); return true; }
    if (auto l = std::dynamic_pointer_cast<jxx::lang::Long>(o)) { out = (long long)l->longValue(); return true; }
    if (auto s = std::dynamic_pointer_cast<jxx::lang::Short>(o)) { out = (long long)s->shortValue(); return true; }
    if (auto b = std::dynamic_pointer_cast<jxx::lang::Byte>(o)) { out = (long long)b->byteValue(); return true; }
    return false;
}

static inline bool has_double(jxx::Ptr<jxx::lang::Object> o, double& out) {
    if (auto f = std::dynamic_pointer_cast<jxx::lang::Float>(o)) { out = (double)f->floatValue(); return true; }
    if (auto d = std::dynamic_pointer_cast<jxx::lang::Double>(o)) { out = (double)d->doubleValue(); return true; }
    long long li=0; if (has_long(o, li)) { out = (double)li; return true; }
    return false;
}

static inline bool has_bool(jxx::Ptr<jxx::lang::Object> o, bool& out) {
    if (auto b = std::dynamic_pointer_cast<jxx::lang::Boolean>(o)) { out = b->booleanValue(); return true; }
    return false;
}

static inline bool has_char(jxx::Ptr<jxx::lang::Object> o, char32_t& out) {
    if (auto c = std::dynamic_pointer_cast<jxx::lang::Character>(o)) { out = (char32_t)c->charValue(); return true; }
    long long li=0; if (has_long(o, li)) { out = (char32_t)li; return true; }
    return false;
}

// Date/Calendar extraction (timezone-aware for Calender)
static inline bool has_epoch_millis(jxx::Ptr<jxx::lang::Object> o, long long& millis, jint& tzOffsetMillis) {
    tzOffsetMillis = 0;
    if (!o) return false;

    if (has_long(o, millis)) return true;

    if (auto d = std::dynamic_pointer_cast<jxx::util::Date>(o)) {
        millis = (long long)d->getTime();
        return true;
    }

    if (auto c = std::dynamic_pointer_cast<jxx::util::Calender>(o)) {
        millis = (long long)c->getTimeInMillis();
        auto tz = c->getTimeZone();
        if (tz) tzOffsetMillis = tz->getOffset((jlong)millis);
        return true;
    }

    return false;
}

static inline std::tm tm_from_epoch_with_offset(std::time_t utcSeconds, int offsetSeconds) {
    std::time_t adj = utcSeconds + offsetSeconds;
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &adj);
#else
    gmtime_r(&adj, &tm);
#endif
    return tm;
}

static inline std::string fmt_tm(const std::tm& tm, const char* pat) {
    std::ostringstream oss;
    oss.imbue(std::locale::classic());
    oss << std::put_time(&tm, pat);
    return oss.str();
}

static inline std::string java_special_fp(double v, bool upper) {
    if (std::isnan(v)) return upper ? "NAN" : "NaN";
    if (std::isinf(v)) {
        if (std::signbit(v)) return upper ? "-INFINITY" : "-Infinity";
        return upper ? "INFINITY" : "Infinity";
    }
    return {};
}

static inline std::string group_decimal(const std::string& s) {
    std::string sign, body=s;
    if (!body.empty() && (body[0]=='+' || body[0]=='-')) { sign=body.substr(0,1); body=body.substr(1); }
    std::string intp=body, frac;
    auto dot=body.find('.');
    if (dot!=std::string::npos) { intp=body.substr(0,dot); frac=body.substr(dot); }
    if (intp.size()<=3) return sign+intp+frac;
    std::string out; out.reserve(body.size()+body.size()/3);
    int cnt=0;
    for (auto it=intp.rbegin(); it!=intp.rend(); ++it) {
        if (cnt==3) { out.push_back(','); cnt=0; }
        out.push_back(*it); ++cnt;
    }
    std::reverse(out.begin(), out.end());
    return sign+out+frac;
}

static inline void apply_width(std::string& out, const FormatSpec& s) {
    if (s.precision >= 0 && (s.conv=='s' || s.conv=='S') && (int)out.size() > s.precision)
        out.resize((std::size_t)s.precision);

    if (s.width <= 0 || (int)out.size() >= s.width) return;
    int pad = s.width - (int)out.size();
    char padChar = (s.zeroPad && !s.leftJustify) ? '0' : ' ';
    if (s.leftJustify) out.append((std::size_t)pad, ' ');
    else out = std::string((std::size_t)pad, padChar) + out;
}

// Parse after '%' (i points at char after '%', returns spec and leaves i at conv)
static void parse_one(const std::string& f, std::size_t& i, FormatSpec& s) {
    // arg index or reuse
    if (i < f.size() && f[i]=='<') { s.reuseLast=true; ++i; }
    else {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        if (i<f.size() && f[i]=='$' && i>start) { s.argIndex=std::stoi(f.substr(start,i-start)); ++i; }
        else i=start;
    }

    // flags
    while (i<f.size()) {
        switch (f[i]) {
            case '-': s.leftJustify=true; ++i; break;
            case '#': s.alt=true; ++i; break;
            case '+': s.plus=true; ++i; break;
            case ' ': s.leadingSpace=true; ++i; break;
            case '0': s.zeroPad=true; ++i; break;
            case ',': s.group=true; ++i; break;
            case '(': s.parens=true; ++i; break;
            case '<': s.reuseLast=true; ++i; break;
            default: goto flags_done;
        }
    }
flags_done:

    // width
    if (i<f.size() && is_digit(f[i])) {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.width = std::stoi(f.substr(start, i-start));
    }

    // precision
    if (i<f.size() && f[i]=='.') {
        ++i;
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.precision = (i==start) ? 0 : std::stoi(f.substr(start, i-start));
    }

    // date/time
    if (i<f.size() && (f[i]=='t' || f[i]=='T')) {
        s.dateTime=true;
        s.upper = (f[i]=='T');
        ++i;
    }

    if (i>=f.size()) throw_fmt("MissingFormatArgumentException");
    s.conv = f[i];
    if (std::isupper((unsigned char)s.conv)) s.upper=true;
}

// date/time formatting subset
static std::string format_dt(char suf, bool upper, long long epochMillis, int tzOffsetMillis) {
    std::time_t utcSec = (std::time_t)(epochMillis / 1000);
    std::tm tm = tm_from_epoch_with_offset(utcSec, tzOffsetMillis/1000);
    int ms = (int)(epochMillis % 1000); if (ms<0) ms += 1000;

    switch (suf) {
        case 'Y': return fmt_tm(tm, "%Y");
        case 'y': return fmt_tm(tm, "%y");
        case 'm': return fmt_tm(tm, "%m");
        case 'd': return fmt_tm(tm, "%d");
        case 'H': return fmt_tm(tm, "%H");
        case 'M': return fmt_tm(tm, "%M");
        case 'S': return fmt_tm(tm, "%S");
        case 'L': { std::ostringstream o; o<<std::setw(3)<<std::setfill('0')<<ms; return o.str(); }
        case 'z': return fmt_tm(tm, "%z");
        case 'Z': return fmt_tm(tm, "%Z");
        case 's': { std::ostringstream o; o<<(epochMillis/1000); return o.str(); }
        case 'Q': { std::ostringstream o; o<<epochMillis; return o.str(); }
        default:
            throw jxx::util::format::UnknownFormatConversionException(
                std::make_shared<jxx::lang::String>(("UnknownFormatConversionException: t"+std::string(1,suf)).c_str()));
    }
}

static std::string format_arg(const FormatSpec& s, jxx::Ptr<jxx::lang::Object> arg) {
    if (s.conv=='%' ) return "%";
    if (s.conv=='n' ) return "\n";

    if (s.dateTime) {
        if (!arg) throw_fmt("NullPointerException");
        long long millis=0; jint off=0;
        if (!has_epoch_millis(arg, millis, off)) throw_fmt("IllegalFormatConversionException: t/T");
        std::string out = format_dt(s.conv, s.upper, millis, off);
        if (s.upper) out = up(out);
        return out;
    }

    char c = s.conv;

    if (c=='s' || c=='S') {
        std::string out = arg ? arg->toString()->utf8() : "null";
        if (s.upper) out = up(out);
        return out;
    }

    if (c=='b' || c=='B') {
        bool bv=false;
        if (!arg) bv=false;
        else if (!has_bool(arg, bv)) bv=true; // Java: non-null => true
        std::string out = bv ? "true" : "false";
        if (s.upper) out = up(out);
        return out;
    }

    if (c=='h' || c=='H') {
        if (!arg) return s.upper ? "NULL" : "null";
        jint hc = arg->hashCode();
        std::ostringstream oss; oss<<std::hex<<(unsigned int)hc;
        std::string out = oss.str();
        if (s.upper) out = up(out);
        return out;
    }

    if (c=='c' || c=='C') {
        if (!arg) throw_fmt("NullPointerException");
        char32_t cp=0;
        if (!has_char(arg, cp)) throw_fmt("IllegalFormatConversionException: c");
        // BMP only best-effort:
        char ch = (cp <= 0x7F) ? (char)cp : '?';
        std::string out(1, ch);
        if (s.upper) out = up(out);
        return out;
    }

    if (c=='d' || c=='o' || c=='x' || c=='X') {
        if (!arg) throw_fmt("NullPointerException");
        long long v=0;
        if (!has_long(arg, v)) throw_fmt("IllegalFormatConversionException: d");
        bool neg = v<0;
        unsigned long long u = neg ? (unsigned long long)(-v) : (unsigned long long)v;
        std::ostringstream oss;
        oss.imbue(std::locale::classic());
        if (c=='d') oss<<u;
        else if (c=='o') { oss<<std::oct<<u; }
        else { oss<<std::hex<<u; }
        std::string out = oss.str();
        if (c=='X') out = up(out);
        if (s.alt && (c=='x' || c=='X')) out = (c=='X' ? "0X":"0x") + out;
        if (s.alt && c=='o' && (out.empty() || out[0] != '0')) out = "0" + out;

        if (neg) out = s.parens ? ("(" + out + ")") : ("-" + out);
        else if (s.plus) out = "+" + out;
        else if (s.leadingSpace) out = " " + out;

        if (s.group && c=='d') out = group_decimal(out);
        return out;
    }

    if (c=='f' || c=='e' || c=='E' || c=='g' || c=='G' || c=='a' || c=='A') {
        if (!arg) throw_fmt("NullPointerException");
        double v=0.0;
        if (!has_double(arg, v)) throw_fmt("IllegalFormatConversionException: f");
        auto sp = java_special_fp(v, s.upper);
        if (!sp.empty()) return sp;

        bool neg = std::signbit(v);
        double av = std::fabs(v);

        int prec = (s.precision >= 0) ? s.precision : 6;
        std::ostringstream oss;
        oss.imbue(std::locale::classic());

        if (c=='f') oss<<std::fixed<<std::setprecision(prec)<<av;
        else if (c=='e' || c=='E') oss<<std::scientific<<std::setprecision(prec)<<av;
        else if (c=='g' || c=='G') oss<<std::setprecision(prec)<<av;
        else { oss<<std::hexfloat<<av; }

        std::string out = oss.str();
        if (c=='E' || c=='G' || c=='A') out = up(out);
        if (s.alt && out.find('.')==std::string::npos) out += ".";
        if (s.group) out = group_decimal(out);

        if (neg) out = s.parens ? ("(" + out + ")") : ("-" + out);
        else if (s.plus) out = "+" + out;
        else if (s.leadingSpace) out = " " + out;

        return out;
    }

    throw jxx::util::format::UnknownFormatConversionException(
        std::make_shared<jxx::lang::String>(("UnknownFormatConversionException: "+std::string(1,c)).c_str()));
}

Formatter::Formatter(jxx::Ptr<jxx::lang::Locale> locale)
    : locale_(locale ? locale : jxx::lang::Locale::getDefault()) {}

jxx::Ptr<jxx::lang::String> Formatter::format(
    jxx::Ptr<jxx::lang::String> fmt,
    jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args) {

    if (!fmt || !args) throw_fmt("NullPointerException");

    std::string f = fmt->utf8();
    std::ostringstream out; out.imbue(std::locale::classic());

    jint nextArg = 1;
    jint lastArg = -1;

    for (std::size_t i=0; i<f.size(); ++i) {
        if (f[i] != '%') { out<<f[i]; continue; }
        if (i+1<f.size() && f[i+1]=='%') { out<<'%'; ++i; continue; }

        ++i;
        FormatSpec spec{};
        parse_one(f, i, spec);

        jint argIndex = -1;
        if (spec.reuseLast) {
            if (lastArg < 0) throw jxx::util::format::MissingFormatArgumentException(
                std::make_shared<jxx::lang::String>("%< without previous"));
            argIndex = lastArg;
        } else if (spec.argIndex > 0) argIndex = spec.argIndex;
        else argIndex = nextArg++;

        lastArg = argIndex;

        if (spec.conv=='n' || spec.conv=='%') {
            std::string chunk = (spec.conv=='n') ? "\n" : "%";
            apply_width(chunk, spec);
            out<<chunk;
            continue;
        }

        if (argIndex <= 0 || (std::uint32_t)argIndex > args->length) {
            throw jxx::util::format::MissingFormatArgumentException(
                std::make_shared<jxx::lang::String>("MissingFormatArgumentException"));
        }

        auto a = (*args)[argIndex - 1];
        std::string chunk = format_arg(spec, a);
        apply_width(chunk, spec);
        out<<chunk;
    }

    return std::make_shared<jxx::lang::String>(out.str().c_str());
}

}