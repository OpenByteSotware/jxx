#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Appendable.h"
#include "io/jxx.io.Flushable.h"
#include "io/jxx.io.Closeable.h"


#include "jxx.util.Locale.h"
#include "jxx.util.FormatterClosedException.h"
#include "jxx.util.UnknownFormatConversionException.h"
#include "jxx.util.MissingFormatArgumentException.h"
#include "jxx.util.IllegalFormatFlagsException.h"
#include "jxx.util.IllegalFormatWidthException.h"
#include "jxx.util.IllegalFormatPrecisionException.h"
#include "jxx.util.Formattable.h"

#include <string>
#include <vector>

namespace jxx::io {
	class IOException;
}

namespace jxx::util {

/**
 * Java 8 parity (practical): java.util.Formatter
 *
 * - Supports core conversions: s/S, d, o, x/X, f, e/E, g/G, a/A, b/B, c/C, h/H, n, %
 * - Supports flags: '-', '+', ' ', '0', ',', '(', '#', '<'
 * - Supports width and precision
 * - Date/time t/T conversions: present but throw UnsupportedOperationException (unless you add time API)
 */
class Formatter final : public jxx::lang::Object, public jxx::io::Flushable, public jxx::io::Closeable {
public:
    Formatter();
    explicit Formatter(jxx::Ptr<jxx::lang::Appendable> a);
    explicit Formatter(jxx::Ptr<jxx::util::Locale> l);
    Formatter(jxx::Ptr<jxx::lang::Appendable> a, jxx::Ptr<jxx::util::Locale> l);

    // Core API
    jxx::Ptr<jxx::lang::Appendable> out() const;
    jxx::Ptr<jxx::util::Locale> locale() const;

    jxx::Ptr<jxx::io::IOException> ioException() const;

    jxx::Ptr<Formatter> format(jxx::Ptr<jxx::lang::String> fmt, jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args);
    jxx::Ptr<Formatter> format(jxx::Ptr<jxx::util::Locale> l, jxx::Ptr<jxx::lang::String> fmt, jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args);

    jxx::Ptr<jxx::lang::String> toString() const override;

    void flush() override;
    void close() override;

private:
    struct Spec {
        // argument index (1-based). 0 means implicit next.
        jxx::lang::jint argIndex = 0;
        jxx::lang::jbool reusePrev = false; // '<'
        std::string flags;
        jxx::lang::jint width = -1;
        jxx::lang::jint precision = -1;
        jxx::lang::jbool dateTime = false; // t/T
        char conv = 0;
        char dt = 0; // date/time suffix
    };

    jxx::Ptr<jxx::lang::Appendable> out_;
    jxx::Ptr<jxx::util::Locale> locale_;
    jxx::Ptr<jxx::io::IOException> lastIo_;
    jxx::lang::jbool closed_ = false;

    // when out_ is null, we use internal string builder
    std::string sb_;

    void ensureOpen_() const;
    void append_(const std::string& s);
    void appendChar_(char c);

    static std::vector<Spec> parse_(const std::string& fmt, std::vector<std::string>& literals);

    static std::string formatOne_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg, jxx::Ptr<jxx::util::Locale> loc);
    static std::string applyWidth_(const std::string& in, const Spec& sp);

    static std::string toLower_(std::string s);

    static std::string formatString_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);
    static std::string formatBoolean_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);
    static std::string formatChar_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);
    static std::string formatHash_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);
    static std::string formatInteger_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);
    static std::string formatFloat_(const Spec& sp, jxx::Ptr<jxx::lang::Object> arg);

    static long long asLongLong_(jxx::Ptr<jxx::lang::Object> arg, bool& ok);
    static long double asLongDouble_(jxx::Ptr<jxx::lang::Object> arg, bool& ok);
};

} // namespace jxx::util
