#include <sstream>
#include <iomanip>
#include <cctype>
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "jxx.util.Formatter.h"

namespace jxx::util {

static bool hasFlag(const std::string& flags, char f) {
    return flags.find(f) != std::string::npos;
}

Formatter::Formatter() : out_(nullptr), locale_(Locale::getDefault()), lastIo_(nullptr), closed_(false) {}

Formatter::Formatter(jxx::Ptr<jxx::lang::Appendable> a)
    : out_(std::move(a)), locale_(Locale::getDefault()), lastIo_(nullptr), closed_(false) {}

Formatter::Formatter(jxx::Ptr<jxx::util::Locale> l)
    : out_(nullptr), locale_(l ? l : Locale::getDefault()), lastIo_(nullptr), closed_(false) {}

Formatter::Formatter(jxx::Ptr<jxx::lang::Appendable> a, jxx::Ptr<jxx::util::Locale> l)
    : out_(std::move(a)), locale_(l ? l : Locale::getDefault()), lastIo_(nullptr), closed_(false) {}

void Formatter::ensureOpen_() const {
    if (closed_) throw FormatterClosedException();
}

jxx::Ptr<jxx::lang::Appendable> Formatter::out() const { return out_; }

jxx::Ptr<jxx::util::Locale> Formatter::locale() const { return locale_; }

jxx::Ptr<jxx::io::IOException> Formatter::ioException() const { return lastIo_; }

std::string Formatter::toLower_(std::string s) {
    for (auto& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

void Formatter::appendChar_(char c) {
    append_(std::string(1, c));
}

void Formatter::append_(const std::string& s) {
    if (!out_) {
        sb_ += s;
        return;
    }
    // append via Appendable using a temporary String
    try {
        out_->append(JXX_NEW<jxx::lang::String>(s.c_str()));
    } catch (const jxx::io::IOException& e) {
        lastIo_ = JXX_NEW<jxx::io::IOException>(e.whatString());
    } catch (...) {
        // keep as null; Formatter in Java stores IOException only
    }
}

jxx::Ptr<jxx::lang::String> Formatter::toString() const {
    if (out_) {
        // If out is a StringBuilder, return its toString(); else return internal snapshot marker.
        auto sb = std::dynamic_pointer_cast<jxx::lang::StringBuilder>(out_);
        if (sb) return sb->toString();
        return JXX_NEW<jxx::lang::String>("(appendable)");
    }
    return JXX_NEW<jxx::lang::String>(sb_.c_str());
}

void Formatter::flush() {
    ensureOpen_();
    // If out is Flushable, flush it.
    auto fl = std::dynamic_pointer_cast<jxx::io::Flushable>(out_);
    if (fl) {
        try { fl->flush(); } catch (...) {}
    }
}

void Formatter::close() {
    if (closed_) return;
    closed_ = true;
    auto cl = std::dynamic_pointer_cast<jxx::io::Closeable>(out_);
    if (cl) {
        try { cl->close(); } catch (...) {}
    }
}

// Parse format string into literals and specs.
// literals.size() == specs.size()+1
std::vector<Formatter::Spec> Formatter::parse_(const std::string& fmt, std::vector<std::string>& literals) {
    std::vector<Spec> specs;
    literals.clear();

    std::string cur;
    std::size_t i = 0;

    auto parseInt = [&](std::size_t& j) -> int {
        int v = 0;
        bool any = false;
        while (j < fmt.size() && std::isdigit((unsigned char)fmt[j])) {
            any = true;
            v = v * 10 + (fmt[j] - '0');
            ++j;
        }
        return any ? v : -1;
    };

    while (i < fmt.size()) {
        char c = fmt[i];
        if (c != '%') {
            cur.push_back(c);
            ++i;
            continue;
        }

        // handle %% and %n
        if (i + 1 < fmt.size() && (fmt[i + 1] == '%' || fmt[i + 1] == 'n')) {
            literals.push_back(cur);
            cur.clear();
            Spec sp;
            sp.conv = fmt[i + 1];
            specs.push_back(sp);
            i += 2;
            continue;
        }

        literals.push_back(cur);
        cur.clear();

        Spec sp;
        std::size_t j = i + 1;

        // argument index or reuse
        if (j < fmt.size() && fmt[j] == '<') {
            sp.reusePrev = true;
            ++j;
        } else {
            // digits then $ optional
            std::size_t save = j;
            int idx = parseInt(j);
            if (idx >= 0 && j < fmt.size() && fmt[j] == '$') {
                sp.argIndex = idx;
                ++j;
            } else {
                j = save;
            }
        }

        // flags
        const std::string flagChars = "-+ 0,(#<";
        while (j < fmt.size() && flagChars.find(fmt[j]) != std::string::npos) {
            // '<' can appear as flag in Java when arg reuse; we accept it
            if (fmt[j] == '<') sp.reusePrev = true;
            else sp.flags.push_back(fmt[j]);
            ++j;
        }

        // width
        int w = parseInt(j);
        if (w >= 0) sp.width = w;

        // precision
        if (j < fmt.size() && fmt[j] == '.') {
            ++j;
            int p = parseInt(j);
            if (p < 0) throw IllegalFormatPrecisionException(-1);
            sp.precision = p;
        }

        // date/time prefix
        if (j < fmt.size() && (fmt[j] == 't' || fmt[j] == 'T')) {
            sp.dateTime = true;
            ++j;
        }

        if (j >= fmt.size()) throw UnknownFormatConversionException(JXX_NEW<jxx::lang::String>(""));
        sp.conv = fmt[j];
        if (sp.dateTime) {
            sp.dt = sp.conv;
            sp.conv = 't';
        }
        ++j;

        specs.push_back(sp);
        i = j;
    }

    literals.push_back(cur);
    return specs;
}

std::string Formatter::applyWidth_(const std::string& in, const Spec& sp) {
    if (sp.width < 0) return in;
    int width = sp.width;
    if ((int)in.size() >= width) return in;

    bool left = hasFlag(sp.flags, '-');
    char pad = hasFlag(sp.flags, '0') && !left ? '0' : ' ';
    std::string out;
    int padCount = width - (int)in.size();
    if (left) {
        out = in;
        out.append((std::size_t)padCount, ' ');
    } else {
        out.assign((std::size_t)padCount, pad);
        out += in;
    }
    return out;
}

long long Formatter::asLongLong_(jxx::Ptr<jxx::lang::Object> arg, bool& ok) {
    ok = false;
    if (!arg) return 0;
    // Try common wrapper types by toString parsing.
    try {
        auto s = arg->toString();
        if (!s) return 0;
        std::string t = s->utf8();
        size_t idx = 0;
        long long v = std::stoll(t, &idx, 10);
        ok = (idx > 0);
        return v;
    } catch (...) {
        return 0;
    }
}

long double Formatter::asLongDouble_(jxx::Ptr<jxx::lang::Object> arg, bool& ok) {
    ok = false;
    if (!arg) return 0;
    try {
        auto s = arg->toString();
        if (!s) return 0;
        std::string t = s->utf8();
        size_t idx = 0;
        long double v = std::stold(t, &idx);
        ok = (idx > 0);
        return v;
    } catch (...) {
        return 0;
    }
}

std::string Formatter::formatString_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    std::string s = arg ? arg->toString()->utf8() : std::string("null");
    if (sp.precision >= 0 && (int)s.size() > sp.precision) s = s.substr(0, (std::size_t)sp.precision);
    if (sp.conv == 'S') {
        for (auto& c : s) c = (char)std::toupper((unsigned char)c);
    }
    return applyWidth_(s, sp);
}

std::string Formatter::formatBoolean_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    std::string s = arg ? "true" : "false";
    if (sp.conv == 'B') {
        for (auto& c : s) c = (char)std::toupper((unsigned char)c);
    }
    return applyWidth_(s, sp);
}

std::string Formatter::formatHash_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    std::ostringstream oss;
    if (!arg) oss << "null";
    else {
        // Use pointer address as hash surrogate
        oss << std::hex << std::nouppercase << (std::uintptr_t)arg.get();
    }
    auto s = oss.str();
    if (sp.conv == 'H') {
        for (auto& c : s) c = (char)std::toupper((unsigned char)c);
    }
    return applyWidth_(s, sp);
}

std::string Formatter::formatChar_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    // Accept Character wrapper via toString; use first UTF-16 code unit.
    std::string out;
    if (!arg) out = "null";
    else {
        auto s = arg->toString();
        auto u16 = s ? s->utf16() : std::u16string();
        char16_t cu = u16.empty() ? 0 : u16[0];
        // encode this single code unit as UTF-8
        if (cu <= 0x7F) out.push_back((char)cu);
        else if (cu <= 0x7FF) {
            out.push_back((char)(0xC0 | ((cu >> 6) & 0x1F)));
            out.push_back((char)(0x80 | (cu & 0x3F)));
        } else {
            out.push_back((char)(0xE0 | ((cu >> 12) & 0x0F)));
            out.push_back((char)(0x80 | ((cu >> 6) & 0x3F)));
            out.push_back((char)(0x80 | (cu & 0x3F)));
        }
    }
    if (sp.conv == 'C') for (auto& c : out) c = (char)std::toupper((unsigned char)c);
    return applyWidth_(out, sp);
}

std::string Formatter::formatInteger_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    bool ok = false;
    long long v = asLongLong_(arg, ok);
    if (!ok) throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("Not an integer"));

    std::ostringstream oss;
    bool upper = (sp.conv == 'X');
    char conv = (char)std::tolower((unsigned char)sp.conv);

    if (conv == 'd') {
        oss << v;
    } else if (conv == 'o') {
        oss << std::oct << (unsigned long long)v;
    } else if (conv == 'x') {
        oss << std::hex << (upper ? std::uppercase : std::nouppercase) << (unsigned long long)v;
    } else {
        throw UnknownFormatConversionException(JXX_NEW<jxx::lang::String>(std::string(1, sp.conv).c_str()));
    }

    std::string s = oss.str();

    // sign flags for decimal
    if (conv == 'd' && v >= 0) {
        if (hasFlag(sp.flags, '+')) s = "+" + s;
        else if (hasFlag(sp.flags, ' ')) s = " " + s;
    }

    // grouping not implemented; ignore ','

    // parentheses for negatives
    if (hasFlag(sp.flags, '(') && v < 0) {
        // s includes '-' already
        if (!s.empty() && s[0] == '-') s = s.substr(1);
        s = "(" + s + ")";
    }

    return applyWidth_(s, sp);
}

std::string Formatter::formatFloat_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg) {
    bool ok = false;
    long double v = asLongDouble_(arg, ok);
    if (!ok) throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("Not a float"));

    std::ostringstream oss;
    oss.imbue(std::locale::classic());

    char conv = (char)std::tolower((unsigned char)sp.conv);

    if (sp.precision >= 0) oss << std::setprecision(sp.precision);

    if (conv == 'f') {
        oss << std::fixed << (double)v;
    } else if (conv == 'e') {
        oss << std::scientific << (double)v;
    } else if (conv == 'g') {
        oss << (double)v;
    } else if (conv == 'a') {
        oss << std::hexfloat << (double)v;
    } else {
        throw UnknownFormatConversionException(JXX_NEW<jxx::lang::String>(std::string(1, sp.conv).c_str()));
    }

    std::string s = oss.str();

    if (sp.conv == 'E' || sp.conv == 'G' || sp.conv == 'A') {
        for (auto& c : s) c = (char)std::toupper((unsigned char)c);
    }

    if (v >= 0) {
        if (hasFlag(sp.flags, '+')) s = "+" + s;
        else if (hasFlag(sp.flags, ' ')) s = " " + s;
    }

    if (hasFlag(sp.flags, '(') && v < 0) {
        if (!s.empty() && s[0] == '-') s = s.substr(1);
        s = "(" + s + ")";
    }

    return applyWidth_(s, sp);
}

std::string Formatter::formatOne_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg, jxx::Ptr<jxx::util::Locale> /*loc*/) {
    if (sp.dateTime) {
        throw jxx::lang::UnsupportedOperationException(JXX_NEW<jxx::lang::String>("Date/time conversions not implemented"));
    }

    switch (sp.conv) {
    case 's': case 'S': return formatString_(sp, arg);
    case 'b': case 'B': return formatBoolean_(sp, arg);
    case 'h': case 'H': return formatHash_(sp, arg);
    case 'c': case 'C': return formatChar_(sp, arg);
    case 'd': case 'o': case 'x': case 'X': return formatInteger_(sp, arg);
    case 'f': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': return formatFloat_(sp, arg);
    default:
        throw UnknownFormatConversionException(JXX_NEW<jxx::lang::String>(std::string(1, sp.conv).c_str()));
    }
}

jxx::Ptr<Formatter> Formatter::format(jxx::Ptr<jxx::lang::String> fmt, jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args) {
    return format(locale_, fmt, args);
}

jxx::Ptr<Formatter> Formatter::format(jxx::Ptr<jxx::util::Locale> l, jxx::Ptr<jxx::lang::String> fmt, jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args) {
    ensureOpen_();
    if (!fmt) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("format"));
    if (!l) l = Locale::getDefault();

    std::string f = fmt->utf8();
    std::vector<std::string> literals;
    auto specs = parse_(f, literals);

    // argument walking
    jxx::lang::jint nextImplicit = 1;
    jxx::lang::jint prevIndex = 0;

    auto argAt = [&](jxx::lang::jint oneBased) -> jxx::Ptr<jxx::lang::Object> {
        if (!args) return nullptr;
        jxx::lang::jint idx = oneBased - 1;
        if (idx < 0 || (std::uint32_t)idx >= args->length) return nullptr;
        return (*args)[idx];
    };

    for (std::size_t i = 0; i < specs.size(); ++i) {
        append_(literals[i]);
        const auto& sp = specs[i];

        if (sp.conv == '%') { appendChar_('%'); continue; }
        if (sp.conv == 'n') { append_("\n"); continue; }

        jxx::lang::jint argIndex;
        if (sp.reusePrev) {
            argIndex = prevIndex;
        } else if (sp.argIndex > 0) {
            argIndex = sp.argIndex;
        } else {
            argIndex = nextImplicit++;
        }
        prevIndex = argIndex;

        auto arg = argAt(argIndex);
        if (!arg && (!args || (std::uint32_t)(argIndex - 1) >= args->length)) {
            throw MissingFormatArgumentException(JXX_NEW<jxx::lang::String>("%"));
        }

        auto piece = formatOne_(sp, arg, l);
        append_(piece);
    }
    append_(literals.back());

    return std::static_pointer_cast<Formatter>(this->thisPtr);
}

} // namespace jxx::util
