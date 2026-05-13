#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.CharSequence.h"
#include "jxx.lang.Comparable.h"
#include "io/jxx.io.Serializable.h"
#include "jxx.lang.Locale.h"
#include "jxx.lang.Charset.h"
#include "jxx.lang.Iterable.h"

namespace jxx::lang {

    // Forward declarations (avoid include cycles)
    class StringBuffer;
    class StringBuilder;
    class Charset; // java.nio.charset.Charset (you have this)
    class Locale;  // java.util.Locale (you have this)
    class ClassAny;

} // namespace jxx::lang

namespace jxx::util {
    class IntStream; // minimal IntStream you requested
}

namespace jxx::lang {

    /**
     * Strict Java 8 parity: java.lang.String
     *
     * Extends: Object
     * Implements: CharSequence, Comparable<String>, Serializable
     *
     * Internal storage: UTF-16 code units (std::u16string)
     */
    class String final
        : public Object
        , public CharSequence
        , public Comparable<String>
        , public jxx::io::Serializable
    {
    public:
        // ---------------------------------------------------------------------
        // Constructors (ALL Java 8 public constructors)
        // ---------------------------------------------------------------------

        // String()
        String();

        // String(String original)
        explicit String(jxx::Ptr<String> original);

        // String(byte[] bytes)
        explicit String(jxx::Ptr<ByteArray> bytes);

        // String(byte[] bytes, int offset, int length)
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length);

        // String(byte[] bytes, String charsetName)
        String(jxx::Ptr<ByteArray> bytes, jxx::Ptr<String> charsetName);

        // String(byte[] bytes, int offset, int length, String charsetName)
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length, jxx::Ptr<String> charsetName);

        // String(byte[] bytes, Charset charset)
        String(jxx::Ptr<ByteArray> bytes, jxx::Ptr<Charset> charset);

        // String(byte[] bytes, int offset, int length, Charset charset)
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length, jxx::Ptr<Charset> charset);

        // Deprecated: String(byte[] ascii, int hibyte)
        String(jxx::Ptr<ByteArray> ascii, jint hibyte);

        // Deprecated: String(byte[] ascii, int hibyte, int offset, int count)
        String(jxx::Ptr<ByteArray> ascii, jint hibyte, jint offset, jint count);

        // String(char[] value)
        explicit String(jxx::Ptr<CharArray> value);

        // String(char[] value, int offset, int count)
        String(jxx::Ptr<CharArray> value, jint offset, jint count);

        // String(int[] codePoints, int offset, int count)
        String(jxx::Ptr<IntArray> codePoints, jint offset, jint count);

        // String(StringBuffer buffer)
        explicit String(jxx::Ptr<StringBuffer> buffer);

        // String(StringBuilder builder)
        explicit String(jxx::Ptr<StringBuilder> builder);

        // JXX convenience (non-Java): String(const char* utf8)
        explicit String(const char* utf8);

        // ---------------------------------------------------------------------
        // CharSequence
        // ---------------------------------------------------------------------
        jint length() const override;
        jchar charAt(jint index) const override;
        jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;

        // Java: String toString()
        virtual jxx::Ptr<String> toString() const override;

        // ---------------------------------------------------------------------
        // Comparable<String>
        // ---------------------------------------------------------------------
        jint compareTo(jxx::Ptr<String> anotherString) const override;

        // ---------------------------------------------------------------------
        // Object overrides
        // ---------------------------------------------------------------------
        jbool equals(jxx::Ptr<Object> obj) const override;
        jint hashCode() const override;

        // ---------------------------------------------------------------------
        // Instance methods (ALL Java 8 public methods)
        // ---------------------------------------------------------------------
        jbool isEmpty() const;

        jint codePointAt(jint index) const;
        jint codePointBefore(jint index) const;
        jint codePointCount(jint beginIndex, jint endIndex) const;
        jint offsetByCodePoints(jint index, jint codePointOffset) const;

        void getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const;

        // Deprecated in Java 8:
        void getBytes(jint srcBegin, jint srcEnd, jxx::Ptr<ByteArray> dst, jint dstBegin) const;

        jxx::Ptr<ByteArray> getBytes() const;
        jxx::Ptr<ByteArray> getBytes(jxx::Ptr<String> charsetName) const;
        jxx::Ptr<ByteArray> getBytes(jxx::Ptr<Charset> charset) const;

        jbool contentEquals(jxx::Ptr<CharSequence> cs) const;
        jbool contentEquals(jxx::Ptr<StringBuffer> sb) const;

        jbool equalsIgnoreCase(jxx::Ptr<String> anotherString) const;
        jint compareToIgnoreCase(jxx::Ptr<String> str) const;

        jbool regionMatches(jint toffset, jxx::Ptr<String> other, jint ooffset, jint len) const;
        jbool regionMatches(jbool ignoreCase, jint toffset, jxx::Ptr<String> other, jint ooffset, jint len) const;

        jbool startsWith(jxx::Ptr<String> prefix) const;
        jbool startsWith(jxx::Ptr<String> prefix, jint toffset) const;
        jbool endsWith(jxx::Ptr<String> suffix) const;

        jint indexOf(jint ch) const;
        jint indexOf(jint ch, jint fromIndex) const;
        jint lastIndexOf(jint ch) const;
        jint lastIndexOf(jint ch, jint fromIndex) const;

        jint indexOf(jxx::Ptr<String> str) const;
        jint indexOf(jxx::Ptr<String> str, jint fromIndex) const;
        jint lastIndexOf(jxx::Ptr<String> str) const;
        jint lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const;

        jxx::Ptr<String> substring(jint beginIndex) const;
        jxx::Ptr<String> substring(jint beginIndex, jint endIndex) const;

        jxx::Ptr<String> concat(jxx::Ptr<String> str) const;

        jxx::Ptr<String> replace(jchar oldChar, jchar newChar) const;

        jbool matches(jxx::Ptr<String> regex) const;
        jbool contains(jxx::Ptr<CharSequence> s) const;

        jxx::Ptr<String> replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replace(jxx::Ptr<CharSequence> target, jxx::Ptr<CharSequence> replacement) const;

        jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex) const;
        jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex, jint limit) const;

        jxx::Ptr<String> toLowerCase() const;
        jxx::Ptr<String> toLowerCase(jxx::Ptr<Locale> locale) const;
        jxx::Ptr<String> toUpperCase() const;
        jxx::Ptr<String> toUpperCase(jxx::Ptr<Locale> locale) const;

        jxx::Ptr<String> trim() const;

        jxx::Ptr<CharArray> toCharArray() const;

        // Java 8 streams:
        jxx::Ptr<jxx::util::IntStream> chars() const;
        jxx::Ptr<jxx::util::IntStream> codePoints() const;

        // Java: String intern()
        jxx::Ptr<String> intern() const;

        // ---------------------------------------------------------------------
        // Static methods (ALL Java 8 public static methods)
        // ---------------------------------------------------------------------
        static jxx::Ptr<String> valueOf(jbool b);
        static jxx::Ptr<String> valueOf(jchar c);
        static jxx::Ptr<String> valueOf(jint i);
        static jxx::Ptr<String> valueOf(jlong l);
        static jxx::Ptr<String> valueOf(jfloat f);
        static jxx::Ptr<String> valueOf(jdouble d);
        static jxx::Ptr<String> valueOf(jxx::Ptr<Object> obj);
        static jxx::Ptr<String> valueOf(jxx::Ptr<CharArray> data);
        static jxx::Ptr<String> valueOf(jxx::Ptr<CharArray> data, jint offset, jint count);

        static jxx::Ptr<String> copyValueOf(jxx::Ptr<CharArray> data);
        static jxx::Ptr<String> copyValueOf(jxx::Ptr<CharArray> data, jint offset, jint count);

        // Java 8 String.format(...) uses Formatter
        static jxx::Ptr<String> format(
            jxx::Ptr<String> format,
            jxx::Ptr<JxxArray<jxx::Ptr<Object>, 1>> args);

        static jxx::Ptr<String> format(
            jxx::Ptr<Locale> l,
            jxx::Ptr<String> format,
            jxx::Ptr<JxxArray<jxx::Ptr<Object>, 1>> args);

        // Java 8 join overloads
        static jxx::Ptr<String> join(
            jxx::Ptr<CharSequence> delimiter,
            jxx::Ptr<JxxArray<jxx::Ptr<CharSequence>, 1>> elements);

        static jxx::Ptr<String> join(
            jxx::Ptr<CharSequence> delimiter,
            jxx::Ptr<jxx::lang::Iterable<jxx::Ptr<CharSequence>>> elements);

        // ---------------------------------------------------------------------
        // Helpers used by runtime (StringBuilder/StringBuffer/etc.)
        // ---------------------------------------------------------------------
        const std::u16string& utf16() const noexcept { return value_; }
        std::string utf8() const;

    private:
        std::u16string value_;
        mutable jint hash_ = 0;
        mutable jbool hashComputed_ = false;

        // Intern pool (Java-like)
        static std::mutex internMutex_;
        static std::unordered_map<std::u16string, std::weak_ptr<String>> internPool_;

        // Conversion helpers (used for regex / UTF-8 interop)
        static std::u16string utf8ToUtf16_(const std::string& s);
        static std::string utf16ToUtf8_(const std::u16string& s);

        static inline char16_t toLowerAscii_(char16_t c);
        static inline char16_t toUpperAscii_(char16_t c);

        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);

        static std::u16string toUtf16_(jxx::Ptr<CharSequence> s);
        static bool isTurkicLocale_(jxx::Ptr<Locale> loc);
    };

} // namespace jxx::lang

