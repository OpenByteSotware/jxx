#include "jxx.util.Formatter.h"

#include "jxx.util.TimeZone.h"
#include "jxx.util.Date.h"
#include "jxx.util.Calendar.h"

#include "jxx.util.format.IllegalFormatException.h"
#include "jxx.util.format.MissingFormatArgumentException.h"
#include "jxx.util.format.UnknownFormatConversionException.h"

#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"

// Wrapper types (adjust include paths if needed)
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
#include <cmath>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

namespace jxx::util {

static inline void throw_fmt(const char* msg) {
    throw jxx::util::format::IllegalFormatException(std::make_shared<jxx::lang::String>(msg));
}
static inline void throw_missing(const char* msg) {
    throw jxx::util::format::MissingFormatArgumentException(std::make_shared<jxx::lang::String>(msg));
}
static inline void throw_unknown(const std::string& msg) {
    throw jxx::util::format::UnknownFormatConversionException(std::make_shared<jxx::lang::String>(msg.c_str()));
}

static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }

static inline std::string to_upper_ascii(std::string s) {
    for (auto& ch : s) ch = (char)std::toupper((unsigned char)ch);
    return s;
}
static inline std::string to_lower_ascii(std::string s) {
    for (auto& ch : s) ch = (char)std::tolower((unsigned char)ch);
    return s;
}

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

struct LocalParts { int year; unsigned mon; unsigned day; int hour; int minute; int second; int millis; int wday_java; };

static inline void civil_from_days(int z, int& y, unsigned& m, unsigned& d) {
    z += 719468;
    const int era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    y = (int)yoe + era * 400;
    const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);
    const unsigned mp = (5*doy + 2)/153;
    d = doy - (153*mp+2)/5 + 1;
    m = mp + (mp < 10 ? 3 : -9);
    y += (m <= 2);
}

static inline LocalParts epoch_to_local_parts(long long epochMillis, int offsetMillis) {
    long long local = epochMillis + (long long)offsetMillis;
    LocalParts p{};
    p.millis = (int)(local % 1000); if (p.millis < 0) p.millis += 1000;
    long long totalSec = local / 1000; if (local < 0 && (local % 1000)) --totalSec;
    long long secOfDay = totalSec % 86400; if (secOfDay < 0) secOfDay += 86400;
    p.hour = (int)(secOfDay / 3600);
    p.minute = (int)((secOfDay % 3600) / 60);
    p.second = (int)(secOfDay % 60);
    long long days = totalSec / 86400; if (totalSec < 0 && (totalSec % 86400)) --days;
    civil_from_days((int)days, p.year, p.mon, p.day);
    int sun0 = (int)((days + 4) % 7); if (sun0 < 0) sun0 += 7;
    p.wday_java = sun0 + 1;
    return p;
}

static inline std::string locale_time_name(const std::locale& loc, const LocalParts& p, const char* fmt) {
    std::tm tm{};
    tm.tm_year = p.year - 1900;
    tm.tm_mon  = (int)p.mon - 1;
    tm.tm_mday = (int)p.day;
    tm.tm_hour = p.hour;
    tm.tm_min  = p.minute;
    tm.tm_sec  = p.second;
    tm.tm_wday = p.wday_java - 1;
    std::ostringstream oss;
    oss.imbue(loc);
    oss << std::put_time(&tm, fmt);
    return oss.str();
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

static void parse_one(const std::string& f, std::size_t& i, FormatSpec& s) {
    // arg index/reuse
    if (i < f.size() && f[i] == '<') { s.reuseLast=true; ++i; }
    else {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        if (i<f.size() && f[i]=='$' && i>start) { s.argIndex=std::stoi(f.substr(start,i-start)); ++i; }
        else i=start;
    }
    // flags (minimal set needed for common Java usage)
    while (i<f.size()) {
        switch (f[i]) {
            case '-': s.leftJustify=true; ++i; break;
            case '0': s.zeroPad=true; ++i; break;
            default: goto flags_done;
        }
    }
flags_done:
    // width
    if (i<f.size() && is_digit(f[i])) {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.width = std::stoi(f.substr(start,i-start));
    }
    // precision
    if (i<f.size() && f[i]=='.') {
        ++i;
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.precision = (i==start) ? 0 : std::stoi(f.substr(start,i-start));
    }
    // datetime
    if (i<f.size() && (f[i]=='t' || f[i]=='T')) { s.dateTime=true; s.upper=(f[i]=='T'); ++i; }
    if (i>=f.size()) throw_missing("MissingFormatArgumentException");
    s.conv=f[i];
    if (std::isupper((unsigned char)s.conv)) s.upper=true;
}

Formatter::Formatter(jxx::Ptr<jxx::lang::Locale> locale)
    : locale_(locale ? locale : jxx::lang::Locale::getDefault()) {}

jxx::Ptr<jxx::lang::String> Formatter::format(jxx::Ptr<jxx::lang::String> fmt,
    jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args) {

    if (!fmt || !args) throw_fmt("NullPointerException");

    std::string f = fmt->utf8();
    std::ostringstream out; out.imbue(std::locale::classic());

    jint nextArg=1;
    jint lastArg=-1;

    const std::locale& loc = locale_->cppLocale();

    for (std::size_t i=0;i<f.size();++i) {
        if (f[i] != '%') { out<<f[i]; continue; }
        if (i+1<f.size() && f[i+1]=='%') { out<<'%'; ++i; continue; }

        ++i;
        FormatSpec spec{};
        parse_one(f, i, spec);

        if (spec.conv=='n') { out<<"\n"; continue; }
        if (spec.conv=='%') { out<<"%"; continue; }

        jint argIndex = -1;
        if (spec.reuseLast) {
            if (lastArg<0) throw_missing("MissingFormatArgumentException");
            argIndex=lastArg;
        } else if (spec.argIndex>0) argIndex=spec.argIndex;
        else argIndex=nextArg++;
        lastArg=argIndex;

        if (argIndex<=0 || (std::uint32_t)argIndex > args->length) throw_missing("MissingFormatArgumentException");
        auto a = (*args)[argIndex-1];

        std::string chunk;

        if (spec.dateTime) {
            if (!a) throw_fmt("NullPointerException");

            long long epoch=0;
            jxx::Ptr<jxx::util::TimeZone> tz;

            if (auto cal = std::dynamic_pointer_cast<jxx::util::Calender>(a)) {
                epoch = (long long)cal->getTimeInMillis();
                tz = cal->getTimeZone();
                if (!tz) tz = jxx::util::TimeZone::getDefault();
            } else if (auto d = std::dynamic_pointer_cast<jxx::util::Date>(a)) {
                epoch = (long long)d->getTime();
                tz = jxx::util::TimeZone::getDefault();
            } else {
                long long v=0;
                if (!has_long(a, v)) throw_fmt("IllegalFormatConversionException: t/T");
                epoch=v; tz=jxx::util::TimeZone::getDefault();
            }

            jint off = tz ? tz->getOffset((jlong)epoch) : 0;
            LocalParts lp = epoch_to_local_parts(epoch, (int)off);

            auto make2 = [](int v){ std::ostringstream o; o<<std::setw(2)<<std::setfill('0')<<v; return o.str(); };
            auto make3 = [](int v){ std::ostringstream o; o<<std::setw(3)<<std::setfill('0')<<v; return o.str(); };
            auto make4 = [](int v){ std::ostringstream o; o<<std::setw(4)<<std::setfill('0')<<v; return o.str(); };

            switch (spec.conv) {
                case 'Y': chunk=make4(lp.year); break;
                case 'm': chunk=make2((int)lp.mon); break;
                case 'd': chunk=make2((int)lp.day); break;
                case 'H': chunk=make2(lp.hour); break;
                case 'M': chunk=make2(lp.minute); break;
                case 'S': chunk=make2(lp.second); break;
                case 'L': chunk=make3(lp.millis); break;
                case 'z': {
                    int offSec = (int)off / 1000;
                    char sign='+'; if (offSec<0) { sign='-'; offSec=-offSec; }
                    int hh=offSec/3600; int mm=(offSec%3600)/60;
                    std::ostringstream o; o<<sign<<std::setw(2)<<std::setfill('0')<<hh<<std::setw(2)<<std::setfill('0')<<mm;
                    chunk=o.str();
                    break;
                }
                case 'Z': chunk = tz ? tz->getAbbreviation((jlong)epoch)->utf8() : std::string("UTC"); break;
                case 'a': chunk = locale_time_name(loc, lp, "%a"); break;
                case 'A': chunk = locale_time_name(loc, lp, "%A"); break;
                case 'b': chunk = locale_time_name(loc, lp, "%b"); break;
                case 'B': chunk = locale_time_name(loc, lp, "%B"); break;
                case 'p': chunk = to_lower_ascii(locale_time_name(loc, lp, "%p")); break;
                default: throw_unknown(std::string("UnknownFormatConversionException: t") + spec.conv);
            }
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='s' || spec.conv=='S') {
            chunk = a ? a->toString()->utf8() : std::string("null");
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='b' || spec.conv=='B') {
            bool bv=false;
            if (!a) bv=false;
            else if (!has_bool(a, bv)) bv=true;
            chunk = bv ? "true" : "false";
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='d') {
            long long v=0; if (!has_long(a, v)) throw_fmt("IllegalFormatConversionException: d");
            chunk = std::to_string(v);

        } else if (spec.conv=='f') {
            double v=0; if (!has_double(a, v)) throw_fmt("IllegalFormatConversionException: f");
            std::ostringstream o; o.imbue(std::locale::classic());
            int prec = (spec.precision>=0) ? spec.precision : 6;
            o<<std::fixed<<std::setprecision(prec)<<v;
            chunk=o.str();

        } else {
            throw_unknown(std::string("UnknownFormatConversionException: ") + spec.conv);
        }

        apply_width(chunk, spec);
        out<<chunk;
    }

    return std::make_shared<jxx::lang::String>(out.str().c_str());
}

} // namespace jxx::util
