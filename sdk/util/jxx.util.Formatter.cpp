#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <fmt/format.h>
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Number.h"
#include "jxx.util.TimeZone.h"
#include "jxx.util.Formatter.h"

namespace jxx::util
{
    void fail_(const char* message)
    {
        throw std::runtime_error(message);
    }

    inline std::string toUtf8OrEmpty_(const jxx::Ptr<jxx::lang::String> s)
    {
        return s ? s->utf8() : std::string{};
    }

    inline std::string toUpperAscii_(std::string s)
    {
        for (char& ch : s)
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        return s;
    }

    inline std::string toLowerAscii_(std::string s)
    {
        for (char& ch : s)
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        return s;
    }

    inline bool isDigit_(char ch) noexcept
    {
        return ch >= '0' && ch <= '9';
    }

    inline std::string repeat_(char ch, int count)
    {
        return count > 0 ? std::string(static_cast<std::size_t>(count), ch) : std::string{};
    }

    inline std::string insertGrouping_(std::string digits)
    {
        if (digits.size() <= 3)
            return digits;

        std::string out;
        const int first = static_cast<int>(digits.size()) % 3;
        std::size_t pos = 0;

        if (first != 0)
        {
            out.append(digits, 0, static_cast<std::size_t>(first));
            pos = static_cast<std::size_t>(first);
            if (pos < digits.size())
                out.push_back(',');
        }

        while (pos < digits.size())
        {
            out.append(digits, pos, 3);
            pos += 3;
            if (pos < digits.size())
                out.push_back(',');
        }

        return out;
    }

    inline std::string groupFloatIntegralPart_(const std::string& s)
    {
        if (s.empty())
            return s;

        std::size_t start = 0;
        std::string prefix;

        if (s[0] == '+' || s[0] == '-' || s[0] == ' ')
        {
            prefix.push_back(s[0]);
            start = 1;
        }

        const auto dot = s.find('.', start);
        const auto expPos = s.find_first_of("eE", start);

        std::size_t intEnd = s.size();
        if (dot != std::string::npos)
            intEnd = dot;
        else if (expPos != std::string::npos)
            intEnd = expPos;

        const std::string integral = s.substr(start, intEnd - start);
        const std::string rest = s.substr(intEnd);

        return prefix + insertGrouping_(integral) + rest;
    }

    inline std::string applyWidth_(std::string s, int width, bool leftJustify, char fill)
    {
        if (width <= 0 || static_cast<int>(s.size()) >= width)
            return s;

        const int pad = width - static_cast<int>(s.size());

        if (leftJustify)
            return s + repeat_(fill, pad);

        return repeat_(fill, pad) + s;
    }

    inline std::string applyNumericWidth_(
        const std::string& prefix,
        const std::string& body,
        int width,
        bool leftJustify,
        bool zeroPad)
    {
        const std::string combined = prefix + body;

        if (width <= 0 || static_cast<int>(combined.size()) >= width)
            return combined;

        const int pad = width - static_cast<int>(combined.size());

        if (leftJustify)
            return combined + repeat_(' ', pad);

        if (zeroPad)
            return prefix + repeat_('0', pad) + body;

        return repeat_(' ', pad) + combined;
    }

    enum class ArgKind
    {
        Null,
        Boolean,
        Integral,
        Floating,
        Character,
        String,
        ObjectString
    };

    struct ArgValue
    {
        ArgKind kind = ArgKind::Null;
        bool boolValue = false;
        long long intValue = 0;
        double floatValue = 0.0;
        char32_t charValue = 0;
        std::string stringValue;
        jxx::Ptr<jxx::lang::Object> objectValue;
    };

    inline std::vector<ArgValue> unboxArgs_(
        const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args)
    {
        std::vector<ArgValue> out;

        if (!args)
            return out;

        const jxx::lang::jint n = args->size();
        out.reserve(static_cast<std::size_t>(n));

        for (jxx::lang::jint i = 0; i < n; ++i)
        {
            const auto& value = (*args)(i);

            ArgValue arg;
            arg.objectValue = value;

            if (!value)
            {
                arg.kind = ArgKind::Null;
            }
            else if (auto s = jxx::CAST<jxx::lang::String, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::String;
                arg.stringValue = s->utf8();
            }
            else if (auto v = jxx::CAST<jxx::lang::Boolean, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Boolean;
                arg.boolValue = static_cast<bool>(v->booleanValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Byte, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Integral;
                arg.intValue = static_cast<long long>(v->byteValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Short, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Integral;
                arg.intValue = static_cast<long long>(v->shortValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Integer, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Integral;
                arg.intValue = static_cast<long long>(v->intValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Long, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Integral;
                arg.intValue = static_cast<long long>(v->longValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Float, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Floating;
                arg.floatValue = static_cast<double>(v->floatValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Double, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Floating;
                arg.floatValue = static_cast<double>(v->doubleValue());
            }
            else if (auto v = jxx::CAST<jxx::lang::Character, jxx::lang::Object>(value))
            {
                arg.kind = ArgKind::Character;
                arg.charValue = static_cast<char32_t>(v->charValue());
            }
            else
            {
                arg.kind = ArgKind::ObjectString;
                const auto s = value->toString();
                arg.stringValue = s ? s->utf8() : std::string("null");
            }

            out.push_back(std::move(arg));
        }

        return out;
    }

    struct Spec
    {
        int argIndex = -1;          // stored 0-based
        bool previous = false;      // %<...
        bool leftJustify = false;   // -
        bool alternate = false;     // #
        bool plus = false;          // +
        bool leadingSpace = false;  // ' '
        bool zeroPad = false;       // 0
        bool grouping = false;      // ,
        bool parentheses = false;   // (
        int width = -1;
        int precision = -1;
        bool dateTime = false;      // t/T prefix
        bool upper = false;         // uppercase output variant
        char conv = '\0';           // normalized lower-case conversion
        char dateSuffix = '\0';     // exact date/time suffix
    };

    inline int parseUnsigned_(const std::string& fmt, std::size_t& i)
    {
        if (i >= fmt.size() || !isDigit_(fmt[i]))
            fail_("expected digits in format specifier");

        int value = 0;
        while (i < fmt.size() && isDigit_(fmt[i]))
        {
            value = value * 10 + (fmt[i] - '0');
            ++i;
        }
        return value;
    }

    inline Spec parseSpec_(const std::string& fmt, std::size_t& i)
    {
        Spec spec{};

        // Optional argument_index$
        {
            const std::size_t save = i;

            if (i < fmt.size() && isDigit_(fmt[i]))
            {
                const int index = parseUnsigned_(fmt, i);
                if (i < fmt.size() && fmt[i] == '$')
                {
                    spec.argIndex = index - 1;
                    ++i;
                }
                else
                {
                    i = save;
                }
            }
        }

        // Flags
        bool doneFlags = false;
        while (!doneFlags && i < fmt.size())
        {
            switch (fmt[i])
            {
            case '-': spec.leftJustify = true; ++i; break;
            case '#': spec.alternate = true; ++i; break;
            case '+': spec.plus = true; ++i; break;
            case ' ': spec.leadingSpace = true; ++i; break;
            case '0': spec.zeroPad = true; ++i; break;
            case ',': spec.grouping = true; ++i; break;
            case '(': spec.parentheses = true; ++i; break;
            case '<': spec.previous = true; ++i; break;
            default: doneFlags = true; break;
            }
        }

        // Width
        if (i < fmt.size() && isDigit_(fmt[i]))
        {
            spec.width = parseUnsigned_(fmt, i);
        }

        // Precision
        if (i < fmt.size() && fmt[i] == '.')
        {
            ++i;
            spec.precision = parseUnsigned_(fmt, i);
        }

        // Date/time prefix
        if (i < fmt.size() && (fmt[i] == 't' || fmt[i] == 'T'))
        {
            spec.dateTime = true;
            spec.upper = (fmt[i] == 'T');
            ++i;

            if (i >= fmt.size())
                fail_("unterminated date/time format specifier");

            spec.dateSuffix = fmt[i++];
            return spec;
        }

        if (i >= fmt.size())
            fail_("unterminated format specifier");

        const char c = fmt[i++];
        spec.conv = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (std::isupper(static_cast<unsigned char>(c)))
            spec.upper = true;

        return spec;
    }

    inline const ArgValue& selectArg_(
        const std::vector<ArgValue>& args,
        const Spec& spec,
        int& nextArg,
        int& lastArg)
    {
        int index = -1;

        if (spec.previous)
        {
            if (lastArg < 0)
                fail_("previous argument flag '<' used without previous argument");
            index = lastArg;
        }
        else if (spec.argIndex >= 0)
        {
            index = spec.argIndex;
        }
        else
        {
            index = nextArg++;
        }

        if (index < 0 || static_cast<std::size_t>(index) >= args.size())
            fail_("missing format argument");

        lastArg = index;
        return args[static_cast<std::size_t>(index)];
    }

    inline std::string objectHashHex_(const ArgValue& arg)
    {
        if (!arg.objectValue)
            return "null";

        const unsigned int hv = static_cast<unsigned int>(arg.objectValue->hashCode());
        return fmt::format("{:x}", hv);
    }

    inline std::string renderString_(const ArgValue& arg, const Spec& spec)
    {
        std::string out;

        switch (arg.kind)
        {
        case ArgKind::Null:
            out = "null";
            break;
        case ArgKind::Boolean:
            out = arg.boolValue ? "true" : "false";
            break;
        case ArgKind::Integral:
            out = fmt::format("{}", arg.intValue);
            break;
        case ArgKind::Floating:
            out = fmt::format("{}", arg.floatValue);
            break;
        case ArgKind::Character:
            out = jxx::lang::String::valueOf(static_cast<jxx::lang::jchar>(arg.charValue))->utf8();
            break;
        case ArgKind::String:
        case ArgKind::ObjectString:
            out = arg.stringValue;
            break;
        }

        if (spec.precision >= 0 && static_cast<int>(out.size()) > spec.precision)
            out.resize(static_cast<std::size_t>(spec.precision));

        if (spec.upper)
            out = toUpperAscii_(std::move(out));

        return applyWidth_(std::move(out), spec.width, spec.leftJustify, ' ');
    }

    inline std::string renderBoolean_(const ArgValue& arg, const Spec& spec)
    {
        std::string out;

        if (arg.kind == ArgKind::Null)
            out = "false";
        else if (arg.kind == ArgKind::Boolean)
            out = arg.boolValue ? "true" : "false";
        else
            out = "true";

        if (spec.upper)
            out = toUpperAscii_(std::move(out));

        return applyWidth_(std::move(out), spec.width, spec.leftJustify, ' ');
    }

    inline std::string renderHash_(const ArgValue& arg, const Spec& spec)
    {
        std::string out = objectHashHex_(arg);

        if (spec.upper)
            out = toUpperAscii_(std::move(out));

        return applyWidth_(std::move(out), spec.width, spec.leftJustify, ' ');
    }

    inline std::string renderChar_(const ArgValue& arg, const Spec& spec)
    {
        char32_t cp = 0;

        switch (arg.kind)
        {
        case ArgKind::Character:
            cp = arg.charValue;
            break;
        case ArgKind::Integral:
            cp = static_cast<char32_t>(arg.intValue);
            break;
        default:
            fail_("illegal argument type for %c");
        }

        std::string out = jxx::lang::String::valueOf(static_cast<jxx::lang::jchar>(cp))->utf8();

        if (spec.upper)
            out = toUpperAscii_(std::move(out));

        return applyWidth_(std::move(out), spec.width, spec.leftJustify, ' ');
    }

    inline std::string applySignAndParens_(
        bool negative,
        const Spec& spec,
        const std::string& digits)
    {
        if (negative)
        {
            if (spec.parentheses)
                return "(" + digits + ")";
            return "-" + digits;
        }

        if (spec.plus)
            return "+" + digits;

        if (spec.leadingSpace)
            return " " + digits;

        return digits;
    }

    template<typename UInt>
    std::string toUnsignedBase_(UInt value, unsigned base, bool upper)
    {
        if (base == 10)
            return fmt::format("{}", value);
        if (base == 8)
            return fmt::format("{:o}", value);
        if (base == 16)
            return upper ? fmt::format("{:X}", value) : fmt::format("{:x}", value);

        fail_("unsupported integral base");
    }

    inline std::string renderIntegral_(const ArgValue& arg, const Spec& spec)
    {
        if (arg.kind != ArgKind::Integral)
            fail_("illegal argument type for integral conversion");

        if (spec.precision >= 0)
            fail_("precision not allowed for integral conversions");

        const long long value = arg.intValue;
        const bool negative = value < 0;

        if (spec.conv != 'd' && spec.conv != 'o' && spec.conv != 'x')
            fail_("unsupported integral conversion");

        unsigned base = 10;
        bool upperHex = false;

        if (spec.conv == 'o')
            base = 8;
        else if (spec.conv == 'x')
        {
            base = 16;
            upperHex = spec.upper;
        }

        const unsigned long long magnitude =
            negative
            ? static_cast<unsigned long long>(-(value + 1LL)) + 1ULL
            : static_cast<unsigned long long>(value);

        std::string digits = toUnsignedBase_(magnitude, base, upperHex);

        if (spec.grouping && spec.conv == 'd')
            digits = insertGrouping_(digits);

        std::string prefix;
        if (spec.alternate)
        {
            if (spec.conv == 'o')
                prefix = "0";
            else if (spec.conv == 'x')
                prefix = spec.upper ? "0X" : "0x";
        }

        const std::string signedBody = applySignAndParens_(negative, spec, digits);

        if (!prefix.empty())
        {
            if (!signedBody.empty() &&
                (signedBody[0] == '+' || signedBody[0] == '-' || signedBody[0] == ' '))
            {
                return applyNumericWidth_(
                    std::string(1, signedBody[0]) + prefix,
                    signedBody.substr(1),
                    spec.width,
                    spec.leftJustify,
                    spec.zeroPad && !spec.leftJustify);
            }

            if (negative && spec.parentheses)
            {
                return applyWidth_(
                    prefix + signedBody,
                    spec.width,
                    spec.leftJustify,
                    spec.zeroPad ? '0' : ' ');
            }

            return applyNumericWidth_(
                prefix,
                signedBody,
                spec.width,
                spec.leftJustify,
                spec.zeroPad && !spec.leftJustify);
        }

        if (!signedBody.empty() &&
            (signedBody[0] == '+' || signedBody[0] == '-' || signedBody[0] == ' '))
        {
            return applyNumericWidth_(
                std::string(1, signedBody[0]),
                signedBody.substr(1),
                spec.width,
                spec.leftJustify,
                spec.zeroPad && !spec.leftJustify);
        }

        if (negative && spec.parentheses)
        {
            return applyWidth_(
                signedBody,
                spec.width,
                spec.leftJustify,
                spec.zeroPad ? '0' : ' ');
        }

        return applyWidth_(
            signedBody,
            spec.width,
            spec.leftJustify,
            spec.zeroPad && !spec.leftJustify ? '0' : ' ');
    }

    inline std::string formatHexFloat_(double absValue, const Spec& spec)
    {
        const char* fmtPattern = nullptr;
        if (spec.precision >= 0)
            fmtPattern = spec.upper ? "%.*A" : "%.*a";
        else
            fmtPattern = spec.upper ? "%A" : "%a";

        const int needed =
            (spec.precision >= 0)
            ? std::snprintf(nullptr, 0, fmtPattern, spec.precision, absValue)
            : std::snprintf(nullptr, 0, fmtPattern, absValue);

        if (needed < 0)
            fail_("hex float formatting failed");

        std::vector<char> buf(static_cast<std::size_t>(needed) + 1U, '\0');

        const int written =
            (spec.precision >= 0)
            ? std::snprintf(buf.data(), buf.size(), fmtPattern, spec.precision, absValue)
            : std::snprintf(buf.data(), buf.size(), fmtPattern, absValue);

        if (written < 0)
            fail_("hex float formatting failed");

        return std::string(buf.data(), static_cast<std::size_t>(written));
    }

    inline std::string formatFloatBody_(double absValue, const Spec& spec)
    {
        const int precision = (spec.precision >= 0) ? spec.precision : 6;

        switch (spec.conv)
        {
        case 'f':
        {
            const std::string fs = fmt::format("{{:.{}f}}", precision);
            return fmt::format(fmt::runtime(fs), absValue);
        }
        case 'e':
        {
            const std::string fs = fmt::format("{{:.{}e}}", precision);
            return fmt::format(fmt::runtime(fs), absValue);
        }
        case 'g':
        {
            const std::string fs = fmt::format("{{:.{}g}}", precision);
            return fmt::format(fmt::runtime(fs), absValue);
        }
        case 'a':
            return formatHexFloat_(absValue, spec);
        default:
            fail_("unsupported floating conversion");
        }
    }

    inline std::string renderFloating_(const ArgValue& arg, const Spec& spec)
    {
        if (arg.kind != ArgKind::Floating && arg.kind != ArgKind::Integral)
            fail_("illegal argument type for floating conversion");

        const double raw = (arg.kind == ArgKind::Floating)
            ? arg.floatValue
            : static_cast<double>(arg.intValue);

        const bool negative = std::signbit(raw);
        const double absValue = negative ? -raw : raw;

        std::string body = formatFloatBody_(absValue, spec);

        if (spec.grouping && spec.conv == 'f')
            body = groupFloatIntegralPart_(body);

        if (spec.upper && spec.conv != 'a')
            body = toUpperAscii_(std::move(body));

        const std::string signedBody = applySignAndParens_(negative, spec, body);

        if (!signedBody.empty() &&
            (signedBody[0] == '+' || signedBody[0] == '-' || signedBody[0] == ' '))
        {
            return applyNumericWidth_(
                std::string(1, signedBody[0]),
                signedBody.substr(1),
                spec.width,
                spec.leftJustify,
                spec.zeroPad && !spec.leftJustify);
        }

        if (negative && spec.parentheses)
        {
            return applyWidth_(
                signedBody,
                spec.width,
                spec.leftJustify,
                spec.zeroPad ? '0' : ' ');
        }

        return applyWidth_(
            signedBody,
            spec.width,
            spec.leftJustify,
            spec.zeroPad && !spec.leftJustify ? '0' : ' ');
    }

    // ---------------------------------------------------------------------
    // Locale resolution using Locale fields directly
    // ---------------------------------------------------------------------

    inline void pushIfNew_(std::vector<std::string>& out, const std::string& value)
    {
        if (value.empty())
            return;

        if (std::find(out.begin(), out.end(), value) == out.end())
            out.push_back(value);
    }

    inline std::string localeField_(const jxx::Ptr<jxx::lang::String>& s)
    {
        return s ? s->utf8() : std::string{};
    }

    inline std::vector<std::string> localeCandidates_(const jxx::Ptr<Locale>& locale)
    {
        std::vector<std::string> names;

        if (!locale)
            return names;

        const std::string language = toLowerAscii_(localeField_(locale->getLanguage()));
        const std::string script = localeField_(locale->getScript());
        const std::string country = localeField_(locale->getCountry());
        const std::string variant = localeField_(locale->getVariant());

        // Try explicit BCP-47 form from Locale first.
        if (const auto tag = locale->toLanguageTag())
        {
            const std::string rawTag = tag->utf8();
            pushIfNew_(names, rawTag);
            pushIfNew_(names, rawTag + ".UTF-8");

            std::string tagUnderscore = rawTag;
            std::replace(tagUnderscore.begin(), tagUnderscore.end(), '-', '_');
            pushIfNew_(names, tagUnderscore);
            pushIfNew_(names, tagUnderscore + ".UTF-8");
        }

        if (!language.empty())
        {
            // language only
            pushIfNew_(names, language);
            pushIfNew_(names, language + ".UTF-8");

            // language-country
            if (!country.empty())
            {
                pushIfNew_(names, language + "_" + country);
                pushIfNew_(names, language + "_" + country + ".UTF-8");
                pushIfNew_(names, language + "-" + country);
                pushIfNew_(names, language + "-" + country + ".UTF-8");
            }

            // language-script-country[-variant]
            std::string composed = language;
            if (!script.empty())
                composed += "-" + script;
            if (!country.empty())
                composed += "-" + country;
            if (!variant.empty())
                composed += "-" + variant;

            pushIfNew_(names, composed);
            pushIfNew_(names, composed + ".UTF-8");

            std::string composedUnderscore = composed;
            std::replace(composedUnderscore.begin(), composedUnderscore.end(), '-', '_');
            pushIfNew_(names, composedUnderscore);
            pushIfNew_(names, composedUnderscore + ".UTF-8");

            // POSIX-ish variant form
            if (!country.empty())
            {
                std::string posix = language + "_" + country;
                if (!variant.empty())
                    posix += "@" + variant;

                pushIfNew_(names, posix);
                pushIfNew_(names, posix + ".UTF-8");
            }
        }

        return names;
    }

    inline std::locale resolveStdLocale_(const jxx::Ptr<Locale>& locale)
    {
        for (const auto& candidate : localeCandidates_(locale))
        {
            try
            {
                return std::locale(candidate.c_str());
            }
            catch (...)
            {
                // try next candidate
            }
        }

        try
        {
            return std::locale("");
        }
        catch (...)
        {
            return std::locale::classic();
        }
    }

    // ---------------------------------------------------------------------
    // TimeZone-backed date/time support
    // ---------------------------------------------------------------------

    inline std::tm gmtTmFromSeconds_(long long epochSeconds)
    {
        const std::time_t tt = static_cast<std::time_t>(epochSeconds);

        std::tm tm{};
#if defined(_WIN32)
        gmtime_s(&tm, &tt);
#else
        gmtime_r(&tt, &tm);
#endif
        return tm;
    }

    inline int millisPart_(long long epochMillis) noexcept
    {
        long long m = epochMillis % 1000LL;
        if (m < 0)
            m += 1000LL;
        return static_cast<int>(m);
    }

    inline jxx::Ptr<jxx::util::TimeZone> defaultTimeZone_()
    {
        try
        {
            return jxx::util::TimeZone::getDefault();
        }
        catch (...)
        {
            return nullptr;
        }
    }

    inline std::tm localTmInTimeZone_(long long epochMillis, const jxx::Ptr<TimeZone>& tz)
    {
        // Convert epochMillis into wall-clock time in the requested timezone by
        // shifting by the zone offset and then interpreting as UTC/GMT.
        const jxx::lang::jint offsetMillis = tz ? tz->getOffset(epochMillis) : 0;
        const long long shiftedMillis = epochMillis + static_cast<long long>(offsetMillis);
        const long long shiftedSeconds = shiftedMillis / 1000LL;
        return gmtTmFromSeconds_(shiftedSeconds);
    }

    inline std::string formatRfc822Offset_(jxx::lang::jint offsetMillis)
    {
        long long totalMinutes = static_cast<long long>(offsetMillis) / 60000LL;
        const char sign = totalMinutes < 0 ? '-' : '+';

        if (totalMinutes < 0)
            totalMinutes = -totalMinutes;

        const long long hours = totalMinutes / 60LL;
        const long long mins = totalMinutes % 60LL;

        return fmt::format("{}{:02}{:02}", sign, hours, mins);
    }

    inline std::string timeZoneAbbreviation_(const jxx::Ptr<TimeZone>& tz, long long epochMillis)
    {
        if (!tz)
            return std::string{};

        const auto abbr = tz->getAbbreviation(epochMillis);
        return abbr ? abbr->utf8() : std::string{};
    }

    inline std::string putTime_(const std::tm& tm, const char* fmtPattern, const std::locale& loc)
    {
        std::ostringstream oss;
        oss.imbue(loc);
        oss << std::put_time(&tm, fmtPattern);
        return oss.str();
    }

    inline std::string dateTimeField_(
        long long epochMillis,
        char suffix,
        bool upperPrefixResult,
        const jxx::Ptr<Locale>& locale)
    {
        const std::locale stdloc = resolveStdLocale_(locale);
        const auto tz = defaultTimeZone_();
        const std::tm tm = localTmInTimeZone_(epochMillis, tz);
        const int millis = millisPart_(epochMillis);
        const jxx::lang::jint offsetMillis = tz ? tz->getOffset(epochMillis) : 0;
        const std::string tzAbbr = timeZoneAbbreviation_(tz, epochMillis);

        std::string out;

        switch (suffix)
        {
            // Time
        case 'H': out = putTime_(tm, "%H", stdloc); break;
        case 'I': out = putTime_(tm, "%I", stdloc); break;
        case 'k':
        {
            const int hour = tm.tm_hour;
            out = (hour < 10) ? fmt::format(" {}", hour) : fmt::format("{}", hour);
            break;
        }
        case 'l':
        {
            int hour = tm.tm_hour % 12;
            if (hour == 0)
                hour = 12;
            out = (hour < 10) ? fmt::format(" {}", hour) : fmt::format("{}", hour);
            break;
        }
        case 'M': out = putTime_(tm, "%M", stdloc); break;
        case 'S': out = putTime_(tm, "%S", stdloc); break;
        case 'L': out = fmt::format("{:03}", millis); break;
        case 'N': out = fmt::format("{:09}", millis * 1000000); break;
        case 'p': out = toLowerAscii_(putTime_(tm, "%p", stdloc)); break;
        case 'z': out = formatRfc822Offset_(offsetMillis); break;
        case 'Z': out = tzAbbr; break;
        case 's': out = fmt::format("{}", epochMillis / 1000LL); break;
        case 'Q': out = fmt::format("{}", epochMillis); break;

            // Date
        case 'B': out = putTime_(tm, "%B", stdloc); break;
        case 'b':
        case 'h': out = putTime_(tm, "%b", stdloc); break;
        case 'A': out = putTime_(tm, "%A", stdloc); break;
        case 'a': out = putTime_(tm, "%a", stdloc); break;
        case 'C': out = fmt::format("{:02}", (tm.tm_year + 1900) / 100); break;
        case 'Y': out = putTime_(tm, "%Y", stdloc); break;
        case 'y': out = putTime_(tm, "%y", stdloc); break;
        case 'j': out = putTime_(tm, "%j", stdloc); break;
        case 'm': out = putTime_(tm, "%m", stdloc); break;
        case 'd': out = putTime_(tm, "%d", stdloc); break;
        case 'e':
            out = (tm.tm_mday < 10) ? fmt::format(" {}", tm.tm_mday)
                : fmt::format("{}", tm.tm_mday);
            break;

            // Composite
        case 'R':
            out = putTime_(tm, "%H:%M", stdloc);
            break;

        case 'T':
            out = putTime_(tm, "%H:%M:%S", stdloc);
            break;

        case 'r':
            out = putTime_(tm, "%I:%M:%S %p", stdloc);
            break;

        case 'D':
            out = putTime_(tm, "%m/%d/%y", stdloc);
            break;

        case 'F':
            out = putTime_(tm, "%Y-%m-%d", stdloc);
            break;

        case 'c':
        {
            // Closer to Java: "Sat Nov 04 12:02:33 EST 1999"
            const std::string left = putTime_(tm, "%a %b %e %H:%M:%S", stdloc);
            if (!tzAbbr.empty())
                out = left + " " + tzAbbr + " " + putTime_(tm, "%Y", stdloc);
            else
                out = left + " " + putTime_(tm, "%Y", stdloc);
            break;
        }

        default:
            fail_("unsupported date/time conversion");
        }

        if (upperPrefixResult)
            out = toUpperAscii_(std::move(out));

        return out;
    }

    inline std::string renderDateTime_(
        const ArgValue& arg,
        const Spec& spec,
        const jxx::Ptr<Locale>& locale)
    {
        if (spec.precision >= 0)
            fail_("precision not allowed for date/time conversions");

        long long epochMillis = 0;

        if (arg.kind == ArgKind::Integral)
        {
            epochMillis = arg.intValue;
        }
        else
        {
            fail_("date/time conversions currently require integral epoch milliseconds");
        }

        return applyWidth_(
            dateTimeField_(epochMillis, spec.dateSuffix, spec.upper, locale),
            spec.width,
            spec.leftJustify,
            ' ');
    }

    inline std::string renderPercent_(const Spec& spec)
    {
        if (spec.precision >= 0)
            fail_("precision not allowed for %% conversion");

        return applyWidth_("%", spec.width, spec.leftJustify, spec.zeroPad ? '0' : ' ');
    }

    inline std::string renderNewline_(const Spec& spec)
    {
        if (spec.width >= 0 || spec.precision >= 0)
            fail_("width/precision not allowed for %n conversion");

        return "\n";
    }

    inline std::string renderOne_(
        const ArgValue* arg,
        const Spec& spec,
        const jxx::Ptr<Locale>& locale)
    {
        if (spec.dateTime)
            return renderDateTime_(*arg, spec, locale);

        switch (spec.conv)
        {
        case 's': return renderString_(*arg, spec);
        case 'b': return renderBoolean_(*arg, spec);
        case 'h': return renderHash_(*arg, spec);
        case 'c': return renderChar_(*arg, spec);
        case 'd':
        case 'o':
        case 'x': return renderIntegral_(*arg, spec);
        case 'e':
        case 'f':
        case 'g':
        case 'a': return renderFloating_(*arg, spec);
        case '%': return renderPercent_(spec);
        case 'n': return renderNewline_(spec);
        default:  fail_("unknown format conversion");
        }
    }

    inline std::string formatJavaLike_(
        const std::string& fmt,
        const std::vector<ArgValue>& args,
        const jxx::Ptr<Locale>& locale)
    {
        std::string out;
        out.reserve(fmt.size() + 32);

        int nextArg = 0;
        int lastArg = -1;

        for (std::size_t i = 0; i < fmt.size(); )
        {
            if (fmt[i] != '%')
            {
                out.push_back(fmt[i]);
                ++i;
                continue;
            }

            ++i; // skip '%'
            const Spec spec = parseSpec_(fmt, i);

            if (!spec.dateTime && (spec.conv == '%' || spec.conv == 'n'))
            {
                out += renderOne_(nullptr, spec, locale);
                continue;
            }

            const ArgValue& arg = selectArg_(args, spec, nextArg, lastArg);
            out += renderOne_(&arg, spec, locale);
        }

        return out;
    }

    inline jxx::Ptr<Formatter> selfPtr_(Formatter* self)
    {
        if (self->thisPtr)
            return std::static_pointer_cast<Formatter>(self->thisPtr);

        return std::static_pointer_cast<Formatter>(self->shared_from_this());
    }
}

namespace jxx::util
{
    Formatter::Formatter()
        : locale_(nullptr)
    {}

    Formatter::Formatter(jxx::Ptr<Locale> locale)
        : locale_(std::move(locale))
    {}

    jxx::Ptr<Locale> Formatter::locale() const noexcept
    {
        return locale_;
    }

    void Formatter::flush()
    {
        ensureOpen_();
        // String-backed sink: nothing to flush.
    }

    void Formatter::close()
    {
        closed_ = true;
    }

    jxx::lang::jbool Formatter::closed() const noexcept
    {
        return closed_;
    }

    void Formatter::ensureOpen_() const
    {
        if (closed_)
            throw std::runtime_error("Formatter is closed");
    }

    jxx::Ptr<Formatter> Formatter::format(
        jxx::Ptr<jxx::lang::String> formatString,
        jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args)
    {
        return format(locale_, std::move(formatString), std::move(args));
    }

    jxx::Ptr<Formatter> Formatter::format(
        jxx::Ptr<Locale> locale,
        jxx::Ptr<jxx::lang::String> formatString,
        jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args)
    {
        ensureOpen_();

        if (locale)
            locale_ = std::move(locale);

        const std::string pattern = toUtf8OrEmpty_(formatString);
        const auto values = unboxArgs_(args);

        buffer_ += formatJavaLike_(pattern, values, locale_);
        return selfPtr_(this);
    }

    jxx::Ptr<jxx::lang::String> Formatter::toString() const
    {
        return jxx::NEW<jxx::lang::String>(buffer_);
    }
}

