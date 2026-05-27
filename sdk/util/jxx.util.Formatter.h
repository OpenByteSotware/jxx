#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <cmath>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Appendable.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"

namespace jxx::lang {
    class String;
    
}

namespace jxx::util {
    class Locale;
}

namespace jxx::io {
	class IOException;
}

namespace jxx::util {
    class Formatter final : public jxx::lang::Object, public jxx::io::Closeable, public jxx::io::Flushable {
    public:
        Formatter();
        explicit Formatter(const jxx::Ptr<jxx::lang::Appendable> a);
        explicit Formatter(const jxx::Ptr<Locale> l);
        Formatter(const jxx::Ptr<jxx::lang::Appendable> a, const jxx::Ptr<Locale> l);
        virtual ~Formatter() override = default;

    public:
        jxx::Ptr<jxx::lang::Appendable> out() const;
        jxx::Ptr<Locale> locale() const;
        jxx::Ptr<jxx::io::IOException> ioException() const;

        jxx::Ptr<Formatter> format(const jxx::Ptr<jxx::lang::String> format,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);

        jxx::Ptr<Formatter> format(const jxx::Ptr<jxx::util::Locale> l,
            const jxx::Ptr<jxx::lang::String> format,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);

        void flush() override;
        void close() override;

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        struct FormatSpec;
        struct ParseState;
        struct ArgCursor;

    private:
        jxx::Ptr<jxx::lang::Appendable> out_;
        jxx::Ptr<Locale> locale_;
        jxx::Ptr<jxx::io::IOException> lastIOException_;
        jxx::lang::jbool closed_;
        std::u16string buffer_;

    private:
        jxx::Ptr<Formatter> self_();
        void ensureOpen_() const;

        void appendUtf16_(const std::u16string& s);
        void appendChar_(char16_t ch);

        static jxx::lang::jint parseDecimal_(const std::u16string& fmt, std::size_t& i);
        static ParseState parseOne_(const std::u16string& fmt, std::size_t start);

        static std::u16string upperAscii_(const std::u16string& s);
        static std::u16string lowerAscii_(const std::u16string& s);
        static std::u16string applyPrecisionToString_(const std::u16string& s, jxx::lang::jint precision);
        static std::u16string applyWidth_(const std::u16string& s, jxx::lang::jbool leftJustify, jxx::lang::jint width, char16_t pad);
        static std::u16string addGrouping_(const std::u16string& s);
        static std::u16string prefixSign_(const std::u16string& magnitude, jxx::lang::jbool negative, const FormatSpec& spec);
        static std::u16string asciiToUtf16_(const std::string& s);
        static std::string utf16ToAscii_(const std::u16string& s);

        static std::u16string objectToString_(const jxx::Ptr<Object> arg);
        static std::u16string objectToBooleanString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper);
        static std::u16string objectToHashString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper);
        static std::u16string objectToCharString_(const jxx::Ptr<Object> arg, jxx::lang::jbool upper);

        static jxx::lang::jbool isFloatingObject_(const jxx::Ptr<Object> arg);
        static long long extractLongLong_(const jxx::Ptr<Object> arg);
        static long double extractLongDouble_(const jxx::Ptr<Object> arg);
        static std::u16string formatIntegral_(const jxx::Ptr<Object> arg, const FormatSpec& spec);
        static std::u16string formatFloating_(const jxx::Ptr<Object> arg, const FormatSpec& spec);
        static std::u16string formatDateTime_(const jxx::Ptr<Object> arg, const FormatSpec& spec);

        static std::u16string formatOne_(const FormatSpec& spec, const jxx::Ptr<Object> arg);
    };
}
