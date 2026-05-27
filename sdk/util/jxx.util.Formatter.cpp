#include "lang/jxx.lang.String.h"
#include "io/jxx.io.IOException.h"
#include "util/jxx.util.Locale.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Character.h"
#include "math/jxx.math.BigInteger.h"
#include "math/jxx.math.BigDecimal.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.Calendar.h"
#include "jxx.util.Formatter.h"

namespace jxx::util
{

    struct Formatter::FormatSpec {
        jxx::lang::jint argumentIndex = -1;      // explicit n$ index, normalized to 0-based
        jxx::lang::jbool reusePrevious = false;  // '<'
        jxx::lang::jbool leftJustify = false;    // '-'
        jxx::lang::jbool alternate = false;      // '#'
        jxx::lang::jbool plus = false;           // '+'
        jxx::lang::jbool leadingSpace = false;   // ' '
        jxx::lang::jbool zeroPad = false;        // '0'
        jxx::lang::jbool group = false;          // ','
        jxx::lang::jbool parentheses = false;    // '('
        jxx::lang::jint width = -1;
        jxx::lang::jint precision = -1;
        char16_t conversion = 0;      // lower-case normalized conversion
        char16_t dateTimeSuffix = 0;  // for %tX
        jxx::lang::jbool upper = false;
    };

    struct Formatter::ParseState {
        FormatSpec spec;
        std::size_t next = 0;
    };

    struct Formatter::ArgCursor {
        jxx::lang::jint nextArg = 0;
        jxx::lang::jint previousArg = -1;
    };

    namespace {
        static std::tm toLocalTm_(std::time_t t)
        {
            std::tm tmv{};
#if defined(_WIN32)
            localtime_s(&tmv, &t);
#else
            localtime_r(&t, &tmv);
#endif
            return tmv;
        }

        static std::tm toGmTm_(std::time_t t)
        {
            std::tm tmv{};
#if defined(_WIN32)
            gmtime_s(&tmv, &t);
#else
            gmtime_r(&t, &tmv);
#endif
            return tmv;
        }

        static std::string twoDigits_(int v)
        {
            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << v;
            return oss.str();
        }

        static std::string threeDigits_(int v)
        {
            std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << v;
            return oss.str();
        }

        static std::string fourDigits_(int v)
        {
            std::ostringstream oss;
            oss << std::setw(4) << std::setfill('0') << v;
            return oss.str();
        }

        static std::string padLeft_(const std::string& s, int width, char fill = '0')
        {
            if (static_cast<int>(s.size()) >= width) {
                return s;
            }
            return std::string(static_cast<std::size_t>(width - static_cast<int>(s.size())), fill) + s;
        }

        static long long extractMillis_(const jxx::Ptr<jxx::lang::Object> arg)
        {
            if (!arg) {
                throw std::runtime_error("null date/time argument");
            }

            jxx::Ptr<Date> d = jxx::CAST<Date, jxx::lang::Object>(arg);
            if (d) {
                return static_cast<long long>(d->getTime());
            }

            jxx::Ptr<Calendar> c = jxx::CAST<Calendar, jxx::lang::Object>(arg);
            if (c) {
                return static_cast<long long>(c->getTimeInMillis());
            }

            jxx::Ptr<jxx::lang::Long> l = jxx::CAST<jxx::lang::Long, jxx::lang::Object>(arg);
            if (l) {
                return static_cast<long long>(l->longValue());
            }

            jxx::Ptr<jxx::lang::Integer> i = jxx::CAST<jxx::lang::Integer, jxx::lang::Object>(arg);
            if (i) {
                return static_cast<long long>(i->longValue());
            }

            throw std::runtime_error("unsupported date/time argument type");
        }

        static std::string weekdayShort_(int wday)
        {
            static const char* names[] = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };
            return (wday >= 0 && wday < 7) ? names[wday] : "";
        }

        static std::string weekdayLong_(int wday)
        {
            static const char* names[] = { "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday" };
            return (wday >= 0 && wday < 7) ? names[wday] : "";
        }

        static std::string monthShort_(int mon)
        {
            static const char* names[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
            return (mon >= 0 && mon < 12) ? names[mon] : "";
        }

        static std::string monthLong_(int mon)
        {
            static const char* names[] = { "January","February","March","April","May","June","July","August","September","October","November","December" };
            return (mon >= 0 && mon < 12) ? names[mon] : "";
        }
    }

    Formatter::Formatter()
        : out_(nullptr),
        locale_(nullptr),
        lastIOException_(nullptr),
        closed_(false),
        buffer_()
    {}

    Formatter::Formatter(const jxx::Ptr<jxx::lang::Appendable> a)
        : out_(a),
        locale_(nullptr),
        lastIOException_(nullptr),
        closed_(false),
        buffer_()
    {}

    Formatter::Formatter(const jxx::Ptr<Locale> l)
        : out_(nullptr),
        locale_(l),
        lastIOException_(nullptr),
        closed_(false),
        buffer_()
    {}

    Formatter::Formatter(const jxx::Ptr<jxx::lang::Appendable> a, const jxx::Ptr<Locale> l)
        : out_(a),
        locale_(l),
        lastIOException_(nullptr),
        closed_(false),
        buffer_()
    {}

    jxx::Ptr<jxx::util::Formatter> Formatter::self_()
    {
        return jxx::CAST<jxx::util::Formatter, jxx::lang::Object>(this);
    }

    jxx::Ptr<jxx::lang::Appendable> Formatter::out() const
    {
        return out_;
    }

    jxx::Ptr<Locale> Formatter::locale() const
    {
        return locale_;
    }

    jxx::Ptr<jxx::io::IOException> Formatter::ioException() const
    {
        return lastIOException_;
    }

    void Formatter::ensureOpen_() const
    {
        if (closed_) {
            throw std::runtime_error("Formatter is closed");
        }
    }

    void Formatter::appendUtf16_(const std::u16string& s)
    {
        ensureOpen_();

        if (!out_) {
            buffer_ += s;
            return;
        }

        try {
            out_->append(jxx::NEW<jxx::lang::String>(s));
        }
        catch (...) {
            throw;
        }
    }

    void Formatter::appendChar_(char16_t ch)
    {
        std::u16string one(1, ch);
        appendUtf16_(one);
    }

    jxx::lang::jint Formatter::parseDecimal_(const std::u16string& fmt, std::size_t& i)
    {
        jxx::lang::jint value = 0;
        bool any = false;

        while (i < fmt.size() && fmt[i] >= u'0' && fmt[i] <= u'9') {
            any = true;
            value = value * 10 + static_cast<jxx::lang::jint>(fmt[i] - u'0');
            ++i;
        }

        return any ? value : -1;
    }

    Formatter::ParseState Formatter::parseOne_(const std::u16string& fmt, std::size_t start)
    {
        ParseState st;
        std::size_t i = start;

        if (i >= fmt.size() || fmt[i] != u'%') {
            throw std::runtime_error("expected %");
        }
        ++i;

        if (i < fmt.size() && fmt[i] == u'%') {
            st.spec.conversion = u'%';
            st.next = i + 1;
            return st;
        }
        if (i < fmt.size() && fmt[i] == u'n') {
            st.spec.conversion = u'n';
            st.next = i + 1;
            return st;
        }

        {
            std::size_t save = i;
            jxx::lang::jint n = parseDecimal_(fmt, i);
            if (n >= 0 && i < fmt.size() && fmt[i] == u'$') {
                st.spec.argumentIndex = n - 1;
                ++i;
            }
            else {
                i = save;
            }
        }

        bool scanningFlags = true;
        while (scanningFlags && i < fmt.size()) {
            switch (fmt[i]) {
            case u'-': st.spec.leftJustify = true; ++i; break;
            case u'#': st.spec.alternate = true; ++i; break;
            case u'+': st.spec.plus = true; ++i; break;
            case u' ': st.spec.leadingSpace = true; ++i; break;
            case u'0': st.spec.zeroPad = true; ++i; break;
            case u',': st.spec.group = true; ++i; break;
            case u'(': st.spec.parentheses = true; ++i; break;
            case u'<': st.spec.reusePrevious = true; ++i; break;
            default: scanningFlags = false; break;
            }
        }

        {
            std::size_t save = i;
            jxx::lang::jint w = parseDecimal_(fmt, i);
            if (w >= 0) {
                st.spec.width = w;
            }
            else {
                i = save;
            }
        }

        if (i < fmt.size() && fmt[i] == u'.') {
            ++i;
            jxx::lang::jint p = parseDecimal_(fmt, i);
            if (p < 0) {
                throw std::runtime_error("illegal precision");
            }
            st.spec.precision = p;
        }

        if (i >= fmt.size()) {
            throw std::runtime_error("missing conversion");
        }

        if (fmt[i] == u't' || fmt[i] == u'T') {
            st.spec.conversion = u't';
            st.spec.upper = (fmt[i] == u'T');
            ++i;
            if (i >= fmt.size()) {
                throw std::runtime_error("missing date/time suffix");
            }
            st.spec.dateTimeSuffix = fmt[i];
            ++i;
            st.next = i;
            return st;
        }

        st.spec.conversion = static_cast<char16_t>(std::tolower(static_cast<unsigned char>(fmt[i])));
        st.spec.upper = (fmt[i] >= u'A' && fmt[i] <= u'Z');
        ++i;
        st.next = i;
        return st;
    }

    std::u16string Formatter::upperAscii_(const std::u16string& s)
    {
        std::u16string out = s;
        for (auto& ch : out) {
            if (ch >= u'a' && ch <= u'z') {
                ch = static_cast<char16_t>(ch - u'a' + u'A');
            }
        }
        return out;
    }

    std::u16string Formatter::lowerAscii_(const std::u16string& s)
    {
        std::u16string out = s;
        for (auto& ch : out) {
            if (ch >= u'A' && ch <= u'Z') {
                ch = static_cast<char16_t>(ch - u'A' + u'a');
            }
        }
        return out;
    }

    std::u16string Formatter::applyPrecisionToString_(const std::u16string& s, jxx::lang::jint precision)
    {
        if (precision < 0 || static_cast<std::size_t>(precision) >= s.size()) {
            return s;
        }
        return s.substr(0, static_cast<std::size_t>(precision));
    }

    std::u16string Formatter::applyWidth_(const std::u16string& s, jxx::lang::jbool leftJustify, jxx::lang::jint width, char16_t pad)
    {
        if (width < 0 || s.size() >= static_cast<std::size_t>(width)) {
            return s;
        }
        std::u16string fill(static_cast<std::size_t>(width) - s.size(), pad);
        return leftJustify ? (s + fill) : (fill + s);
    }

    std::u16string Formatter::addGrouping_(const std::u16string& s)
    {
        if (s.empty()) {
            return s;
        }

        std::u16string sign;
        std::u16string body = s;
        if (!body.empty() && (body[0] == u'+' || body[0] == u'-')) {
            sign.push_back(body[0]);
            body.erase(body.begin());
        }

        std::u16string intPart = body;
        std::u16string fracPart;
        std::size_t dot = body.find(u'.');
        if (dot != std::u16string::npos) {
            intPart = body.substr(0, dot);
            fracPart = body.substr(dot);
        }

        std::u16string grouped;
        for (std::size_t i = 0; i < intPart.size(); ++i) {
            if (i > 0 && ((intPart.size() - i) % 3 == 0)) {
                grouped.push_back(u',');
            }
            grouped.push_back(intPart[i]);
        }

        return sign + grouped + fracPart;
    }

    std::u16string Formatter::prefixSign_(const std::u16string& magnitude, jxx::lang::jbool negative, const FormatSpec& spec)
    {
        if (negative) {
            if (spec.parentheses) {
                return std::u16string(1, u'(') + magnitude + std::u16string(1, u')');
            }
            return std::u16string(1, u'-') + magnitude;
        }
        if (spec.plus) {
            return std::u16string(1, u'+') + magnitude;
        }
        if (spec.leadingSpace) {
            return std::u16string(1, u' ') + magnitude;
        }
        return magnitude;
    }

    std::u16string Formatter::asciiToUtf16_(const std::string& s)
    {
        std::u16string out;
        out.reserve(s.size());
        for (char c : s) {
            out.push_back(static_cast<char16_t>(static_cast<unsigned char>(c)));
        }
        return out;
    }

    std::string Formatter::utf16ToAscii_(const std::u16string& s)
    {
        std::string out;
        out.reserve(s.size());
        for (char16_t c : s) {
            out.push_back(static_cast<char>(c <= 0x7f ? c : '?'));
        }
        return out;
    }

    std::u16string Formatter::objectToString_(const jxx::Ptr<Object> arg)
    {
        if (!arg) {
            return u"null";
        }
        return arg->toString()->utf16();
    }

    std::u16string Formatter::objectToBooleanString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper)
    {
        std::u16string out = arg ? u"true" : u"false";
        return upper ? upperAscii_(out) : out;
    }

    std::u16string Formatter::objectToHashString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper)
    {
        if (!arg) {
            return upper ? u"NULL" : u"null";
        }

        std::ostringstream oss;
        oss << std::hex << std::nouppercase << static_cast<unsigned int>(arg->hashCode());
        std::u16string out = asciiToUtf16_(oss.str());
        return upper ? upperAscii_(out) : out;
    }

    std::u16string Formatter::objectToCharString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper)
    {
        if (!arg) {
            throw std::runtime_error("null argument for %c");
        }

        jxx::lang::jchar ch = 0;

        jxx::Ptr<jxx::lang::Character> c = jxx::CAST<jxx::lang::Character, Object>(arg);
        if (c) {
            ch = c->charValue();
        }
        else {
            long long value = extractLongLong_(arg);
            if (value < 0 || value > 0x10ffff) {
                throw std::runtime_error("illegal code point");
            }
            ch = static_cast<jxx::lang::jchar>(value);
        }

        if (upper) {
            ch = jxx::lang::Character::toUpperCase(ch);
        }

        return std::u16string(1, static_cast<char16_t>(ch));
    }

    jxx::lang::jbool Formatter::isFloatingObject_(const jxx::Ptr<Object> arg)
    {
        return jxx::CAST<jxx::lang::Float, Object>(arg)
            || jxx::CAST<jxx::lang::Double, Object>(arg)
            || jxx::CAST<jxx::math::BigDecimal, Object>(arg);
    }

    long long Formatter::extractLongLong_(const jxx::Ptr<Object> arg)
    {
        if (!arg) {
            throw std::runtime_error("null integral argument");
        }

        jxx::Ptr<jxx::lang::Byte> b = jxx::CAST<jxx::lang::Byte, Object>(arg);
        if (b) return static_cast<long long>(b->longValue());
        jxx::Ptr<jxx::lang::Short> s = jxx::CAST<jxx::lang::Short, Object>(arg);
        if (s) return static_cast<long long>(s->longValue());
        jxx::Ptr<jxx::lang::Integer> i = jxx::CAST<jxx::lang::Integer, Object>(arg);
        if (i) return static_cast<long long>(i->longValue());
        jxx::Ptr<jxx::lang::Long> l = jxx::CAST<jxx::lang::Long, Object>(arg);
        if (l) return static_cast<long long>(l->longValue());

        jxx::Ptr<jxx::lang::Number> n = jxx::CAST<jxx::lang::Number, Object>(arg);
        if (n) return static_cast<long long>(n->longValue());

        throw std::runtime_error("unsupported integral argument type");
    }

    long double Formatter::extractLongDouble_(const jxx::Ptr<Object> arg)
    {
        if (!arg) {
            throw std::runtime_error("null floating argument");
        }

        jxx::Ptr<jxx::lang::Float> f = jxx::CAST<jxx::lang::Float, Object>(arg);
        if (f) return static_cast<long double>(f->doubleValue());
        jxx::Ptr<jxx::lang::Double> d = jxx::CAST<jxx::lang::Double, Object>(arg);
        if (d) return static_cast<long double>(d->doubleValue());
        jxx::Ptr<jxx::math::BigDecimal> bd = jxx::CAST<jxx::math::BigDecimal, Object>(arg);
        if (bd) return static_cast<long double>(bd->doubleValue());

        jxx::Ptr<jxx::lang::Number> n = jxx::CAST<jxx::lang::Number, Object>(arg);
        if (n) return static_cast<long double>(n->doubleValue());

        throw std::runtime_error("unsupported floating argument type");
    }

    std::u16string Formatter::formatIntegral_(const jxx::Ptr<Object> arg, const FormatSpec& spec)
    {
        if (!arg) {
            throw std::runtime_error("null integral argument");
        }

        std::u16string mag;
        jxx::lang::jbool negative = false;

        if (jxx::Ptr<jxx::math::BigInteger> bi = jxx::CAST<jxx::math::BigInteger, Object>(arg)) {
            jxx::lang::jint radix = (spec.conversion == u'o') ? 8 : ((spec.conversion == u'x') ? 16 : 10);
            mag = bi->toString(radix)->utf16();
            if (!mag.empty() && mag[0] == u'-') {
                negative = true;
                mag.erase(mag.begin());
            }
        }
        else {
            long long v = extractLongLong_(arg);
            negative = (v < 0 && spec.conversion == u'd');

            std::ostringstream oss;
            if (spec.conversion == u'd') {
                unsigned long long magv = negative ? static_cast<unsigned long long>(-(v + 1)) + 1ULL : static_cast<unsigned long long>(v);
                oss << magv;
            }
            else if (spec.conversion == u'o') {
                oss << std::oct << static_cast<unsigned long long>(v);
            }
            else {
                oss << std::hex << static_cast<unsigned long long>(v);
            }
            mag = asciiToUtf16_(oss.str());
        }

        if (spec.group && spec.conversion == u'd') {
            mag = addGrouping_(mag);
        }

        if (spec.upper) {
            mag = upperAscii_(mag);
        }

        if (spec.alternate) {
            if (spec.conversion == u'o' && !mag.empty() && mag[0] != u'0') {
                mag = std::u16string(1, u'0') + mag;
            }
            else if (spec.conversion == u'x') {
                mag = std::u16string(spec.upper ? u"0X" : u"0x") + mag;
            }
        }

        std::u16string withSign = (spec.conversion == u'd') ? prefixSign_(mag, negative, spec) : mag;

        if (spec.zeroPad && spec.width > 0 && !spec.leftJustify && withSign.size() < static_cast<std::size_t>(spec.width)) {
            std::u16string prefix;
            std::u16string rest = withSign;
            if (!rest.empty() && (rest[0] == u'+' || rest[0] == u'-' || rest[0] == u' ')) {
                prefix.push_back(rest[0]);
                rest.erase(rest.begin());
            }
            else if (rest.size() >= 2 && rest[0] == u'0' && (rest[1] == u'x' || rest[1] == u'X')) {
                prefix = rest.substr(0, 2);
                rest = rest.substr(2);
            }
            else if (!rest.empty() && rest[0] == u'(' && rest.back() == u')') {
                prefix = u"(";
                rest = rest.substr(1, rest.size() - 2);
                std::u16string zeros(static_cast<std::size_t>(spec.width) - prefix.size() - rest.size() - 1, u'0');
                return prefix + zeros + rest + u")";
            }
            std::u16string zeros(static_cast<std::size_t>(spec.width) - prefix.size() - rest.size(), u'0');
            return prefix + zeros + rest;
        }

        return applyWidth_(withSign, spec.leftJustify, spec.width, u' ');
    }

    std::u16string Formatter::formatFloating_(const jxx::Ptr<Object> arg, const FormatSpec& spec)
    {
        long double value = extractLongDouble_(arg);
        bool negative = std::signbit(static_cast<double>(value));
        long double mag = negative ? -value : value;

        std::ostringstream oss;
        if (spec.precision >= 0) {
            oss << std::setprecision(spec.precision);
        }

        switch (spec.conversion) {
        case u'e': oss << std::scientific; break;
        case u'f': oss << std::fixed; break;
        case u'g': oss << std::defaultfloat; break;
        case u'a': oss << std::hexfloat; break;
        default: break;
        }

        oss << static_cast<long double>(mag);
        std::string raw = oss.str();

        if (spec.conversion == u'g' && spec.precision < 0) {
            // Java default precision is 6 for general floating-point conversions
            std::ostringstream oss2;
            oss2 << std::setprecision(6) << std::defaultfloat << static_cast<long double>(mag);
            raw = oss2.str();
        }

        std::u16string body = asciiToUtf16_(raw);
        if (spec.group && spec.conversion == u'f') {
            body = addGrouping_(body);
        }
        if (spec.upper) {
            body = upperAscii_(body);
        }

        std::u16string withSign = prefixSign_(body, negative, spec);

        if (spec.zeroPad && spec.width > 0 && !spec.leftJustify && withSign.size() < static_cast<std::size_t>(spec.width)) {
            std::u16string prefix;
            std::u16string rest = withSign;
            if (!rest.empty() && (rest[0] == u'+' || rest[0] == u'-' || rest[0] == u' ')) {
                prefix.push_back(rest[0]);
                rest.erase(rest.begin());
            }
            else if (!rest.empty() && rest[0] == u'(' && rest.back() == u')') {
                prefix = u"(";
                rest = rest.substr(1, rest.size() - 2);
                std::u16string zeros(static_cast<std::size_t>(spec.width) - prefix.size() - rest.size() - 1, u'0');
                return prefix + zeros + rest + u")";
            }
            std::u16string zeros(static_cast<std::size_t>(spec.width) - prefix.size() - rest.size(), u'0');
            return prefix + zeros + rest;
        }

        return applyWidth_(withSign, spec.leftJustify, spec.width, u' ');
    }

    std::u16string Formatter::formatDateTime_(const jxx::Ptr<Object> arg, const FormatSpec& spec)
    {
        long long millis = extractMillis_(arg);
        std::time_t sec = static_cast<std::time_t>(millis / 1000LL);
        int milli = static_cast<int>(std::llabs(millis % 1000LL));
        std::tm tmv = toLocalTm_(sec);
        std::tm gmv = toGmTm_(sec);

        char16_t c = spec.dateTimeSuffix;
        std::string out;

        switch (c) {
        case u'H': out = twoDigits_(tmv.tm_hour); break;
        case u'I': { int h = tmv.tm_hour % 12; if (h == 0) h = 12; out = twoDigits_(h); break; }
        case u'k': out = padLeft_(std::to_string(tmv.tm_hour), 2, ' '); break;
        case u'l': { int h = tmv.tm_hour % 12; if (h == 0) h = 12; out = padLeft_(std::to_string(h), 2, ' '); break; }
        case u'M': out = twoDigits_(tmv.tm_min); break;
        case u'S': out = twoDigits_(tmv.tm_sec); break;
        case u'L': out = threeDigits_(milli); break;
        case u'N': out = threeDigits_(milli) + std::string("000000"); break;
        case u'p': out = (tmv.tm_hour < 12) ? "am" : "pm"; break;
        case u'B': out = monthLong_(tmv.tm_mon); break;
        case u'b':
        case u'h': out = monthShort_(tmv.tm_mon); break;
        case u'A': out = weekdayLong_(tmv.tm_wday); break;
        case u'a': out = weekdayShort_(tmv.tm_wday); break;
        case u'C': out = twoDigits_((tmv.tm_year + 1900) / 100); break;
        case u'Y': out = fourDigits_(tmv.tm_year + 1900); break;
        case u'y': out = twoDigits_((tmv.tm_year + 1900) % 100); break;
        case u'j': out = std::string(3 - std::min<int>(3, static_cast<int>(std::to_string(tmv.tm_yday + 1).size())), '0') + std::to_string(tmv.tm_yday + 1); break;
        case u'm': out = twoDigits_(tmv.tm_mon + 1); break;
        case u'd': out = twoDigits_(tmv.tm_mday); break;
        case u'e': out = padLeft_(std::to_string(tmv.tm_mday), 2, ' '); break;
        case u'R': out = twoDigits_(tmv.tm_hour) + ":" + twoDigits_(tmv.tm_min); break;
        case u'T': out = twoDigits_(tmv.tm_hour) + ":" + twoDigits_(tmv.tm_min) + ":" + twoDigits_(tmv.tm_sec); break;
        case u'r': { int h = tmv.tm_hour % 12; if (h == 0) h = 12; out = twoDigits_(h) + ":" + twoDigits_(tmv.tm_min) + ":" + twoDigits_(tmv.tm_sec) + (tmv.tm_hour < 12 ? " AM" : " PM"); break; }
        case u'D': out = twoDigits_(tmv.tm_mon + 1) + "/" + twoDigits_(tmv.tm_mday) + "/" + twoDigits_((tmv.tm_year + 1900) % 100); break;
        case u'F': out = fourDigits_(tmv.tm_year + 1900) + "-" + twoDigits_(tmv.tm_mon + 1) + "-" + twoDigits_(tmv.tm_mday); break;
        case u'c': out = weekdayShort_(tmv.tm_wday) + std::string(" ") + monthShort_(tmv.tm_mon) + std::string(" ") + twoDigits_(tmv.tm_mday) + std::string(" ") + twoDigits_(tmv.tm_hour) + ":" + twoDigits_(tmv.tm_min) + ":" + twoDigits_(tmv.tm_sec) + std::string(" ") + fourDigits_(tmv.tm_year + 1900); break;
        case u's': out = std::to_string(static_cast<long long>(sec)); break;
        case u'Q': out = std::to_string(millis); break;
        case u'z': {
            std::time_t gsec = std::mktime(&gmv);
            std::time_t lsec = std::mktime(&tmv);
            long diff = static_cast<long>(std::difftime(lsec, gsec));
            char sign = diff < 0 ? '-' : '+';
            diff = std::labs(diff);
            int hh = static_cast<int>(diff / 3600);
            int mm = static_cast<int>((diff % 3600) / 60);
            out.push_back(sign);
            out += twoDigits_(hh);
            out += twoDigits_(mm);
            break;
        }
        case u'Z': out = "LOCAL"; break;
        default: throw std::runtime_error("unsupported date/time suffix");
        }

        std::u16string u = asciiToUtf16_(out);
        if (spec.upper) {
            u = upperAscii_(u);
        }
        return applyWidth_(u, spec.leftJustify, spec.width, u' ');
    }

    std::u16string Formatter::formatOne_(const FormatSpec& spec, const jxx::Ptr<Object> arg)
    {
        switch (spec.conversion) {
        case u's': {
            std::u16string out = objectToString_(arg);
            if (spec.upper) {
                out = upperAscii_(out);
            }
            out = applyPrecisionToString_(out, spec.precision);
            return applyWidth_(out, spec.leftJustify, spec.width, u' ');
        }
        case u'b': {
            std::u16string out = objectToBooleanString_(arg, spec.upper);
            out = applyPrecisionToString_(out, spec.precision);
            return applyWidth_(out, spec.leftJustify, spec.width, u' ');
        }
        case u'h': {
            std::u16string out = objectToHashString_(arg, spec.upper);
            return applyWidth_(out, spec.leftJustify, spec.width, u' ');
        }
        case u'c': {
            std::u16string out = objectToCharString_(arg, spec.upper);
            return applyWidth_(out, spec.leftJustify, spec.width, u' ');
        }
        case u'd':
        case u'o':
        case u'x':
            return formatIntegral_(arg, spec);
        case u'e':
        case u'f':
        case u'g':
        case u'a':
            return formatFloating_(arg, spec);
        case u't':
            return formatDateTime_(arg, spec);
        case u'%':
            return applyWidth_(u"%", spec.leftJustify, spec.width, u' ');
        case u'n':
            return u"\n";
        default:
            throw std::runtime_error("unknown conversion");
        }
    }

    jxx::Ptr<Formatter> Formatter::format(
        const jxx::Ptr<jxx::lang::String> format,
        const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args)
    {
        return this->format(locale_, format, args);
    }

    jxx::Ptr<Formatter> Formatter::format(
        const jxx::Ptr<jxx::util::Locale> l,
        const jxx::Ptr<jxx::lang::String> format,
        const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args)
    {
        ensureOpen_();

        if (l) {
            locale_ = l;
        }

        const std::u16string fmt = format ? format->utf16() : std::u16string();
        ArgCursor cursor;

        std::size_t i = 0;
        while (i < fmt.size()) {
            if (fmt[i] != u'%') {
                appendChar_(fmt[i]);
                ++i;
                continue;
            }

            ParseState ps = parseOne_(fmt, i);
            const FormatSpec& spec = ps.spec;

            if (spec.conversion == u'%' || spec.conversion == u'n') {
                appendUtf16_(formatOne_(spec, nullptr));
                i = ps.next;
                continue;
            }

            jxx::lang::jint argIndex = -1;
            if (spec.reusePrevious) {
                if (cursor.previousArg < 0) {
                    throw std::runtime_error("missing previous argument");
                }
                argIndex = cursor.previousArg;
            }
            else if (spec.argumentIndex >= 0) {
                argIndex = spec.argumentIndex;
                cursor.previousArg = argIndex;
            }
            else {
                argIndex = cursor.nextArg++;
                cursor.previousArg = argIndex;
            }

            jxx::Ptr<Object> arg;
            if (args && argIndex >= 0 && argIndex < args->length()) {
                arg = args->get(argIndex);
            }
            else {
                throw std::runtime_error("missing format argument");
            }

            appendUtf16_(formatOne_(spec, arg));
            i = ps.next;
        }

        return self_();
    }

    void Formatter::flush()
    {
        ensureOpen_();
        if (!out_) {
            return;
        }

        jxx::Ptr<jxx::io::Flushable> f = jxx::CAST<jxx::io::Flushable, jxx::lang::Object>(out_);
        if (f) {
            f->flush();
        }
    }

    void Formatter::close()
    {
        if (closed_) {
            return;
        }

        if (out_) {
            jxx::Ptr<jxx::io::Closeable> c = jxx::CAST<jxx::io::Closeable, jxx::lang::Object>(out_);
            if (c) {
                c->close();
            }
        }

        closed_ = true;
    }

    jxx::Ptr<jxx::lang::String> Formatter::toString() const
    {
        if (out_) {
            jxx::Ptr<jxx::lang::Object> o = jxx::CAST<jxx::lang::Object, jxx::lang::Appendable>(out_);
            if (o) {
                return o->toString();
            }
        }
        return jxx::NEW<jxx::lang::String>(buffer_);
    }
}