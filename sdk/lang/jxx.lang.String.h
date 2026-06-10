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
#include "util/jxx.util.Locale.h"
#include "jxx.lang.Charset.h"

namespace jxx::lang {

    // Forward declarations (avoid include cycles)
    class StringBuffer;
    class StringBuilder;
    class Charset; // java.nio.charset.Charset (you have this)
    class ClassAny;
    class String;

	template <typename T>
	class Iterable;

} // namespace jxx::lang

namespace jxx::util {
    class IntStream;
	class Locale;    
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
    class String final : public Object
        , public CharSequence
        , public Comparable<String>
        , public jxx::io::Serializable
    {
    public:

        // String()
        String();

        // String(String original)
        explicit String(const jxx::Ptr<String> original);

        // String(byte[] bytes)
        explicit String(const jxx::lang::ByteArray bytes);

        // String(byte[] bytes, int offset, int length)
        String(const jxx::lang::ByteArray bytes, jxx::lang::jint offset, jxx::lang::jint length);

        // String(byte[] bytes, String charsetName)
        String(const jxx::lang::ByteArray bytes, jxx::Ptr<String> charsetName);

        // String(byte[] bytes, int offset, int length, String charsetName)
        String(const jxx::lang::ByteArray bytes, jxx::lang::jint offset, jxx::lang::jint length, jxx::Ptr<String> charsetName);

        // String(byte[] bytes, Charset charset)
        String(const jxx::lang::ByteArray bytes, jxx::Ptr<Charset> charset);

        // String(byte[] bytes, int offset, int length, Charset charset)
        String(const jxx::lang::ByteArray bytes, jxx::lang::jint offset, jxx::lang::jint length, jxx::Ptr<Charset> charset);

        // Deprecated: String(byte[] ascii, int hibyte)
        String(const jxx::lang::ByteArray ascii, jxx::lang::jint hibyte);

        // Deprecated: String(byte[] ascii, int hibyte, int offset, int count)
        String(const jxx::lang::ByteArray ascii, jxx::lang::jint hibyte, jxx::lang::jint offset, jxx::lang::jint count);

        // String(char[] value)
        explicit String(const CharArray value);

        // String(char[] value, int offset, int count)
        String(const CharArray value, jxx::lang::jint offset, jxx::lang::jint count);

        // String(int[] codePoints, int offset, int count)
        String(const IntArray codePoints, jxx::lang::jint offset, jxx::lang::jint count);

        // String(StringBuffer buffer)
        explicit String(const jxx::Ptr<StringBuffer> buffer);

        // String(StringBuilder builder)
        explicit String(const jxx::Ptr<StringBuilder> builder);

        // JXX convenience (non-Java): String(const char* utf8)
        explicit String(const char* utf8);

        explicit String(const std::string& stdString);
        explicit String(const std::u16string& stdString);

        // ---------------------------------------------------------------------
        // CharSequence
        // ---------------------------------------------------------------------
        jxx::lang::jint length() const override;
        jxx::lang::jchar charAt(jxx::lang::jint index) const override;
        jxx::Ptr<CharSequence> subSequence(jxx::lang::jint start, jxx::lang::jint end) const override;

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

        jxx::lang::jint codePointAt(jxx::lang::jint index) const;
        jxx::lang::jint codePointBefore(jxx::lang::jint index) const;
        jxx::lang::jint codePointCount(jxx::lang::jint beginIndex, jxx::lang::jint endIndex) const;
        jxx::lang::jint offsetByCodePoints(jxx::lang::jint index, jxx::lang::jint codePointOffset) const;

        void getChars(jxx::lang::jint srcBegin, jxx::lang::jint srcEnd, const CharArray dst, jxx::lang::jint dstBegin) const;

        // Deprecated in Java 8:
        void getBytes(jxx::lang::jint srcBegin, jxx::lang::jint srcEnd, const jxx::lang::ByteArray dst, jxx::lang::jint dstBegin) const;

        jxx::lang::ByteArray getBytes() const;
        jxx::lang::ByteArray getBytes(jxx::Ptr<String> charsetName) const;
        jxx::lang::ByteArray getBytes(jxx::Ptr<Charset> charset) const;

        jbool contentEquals(jxx::Ptr<CharSequence> cs) const;
        jbool contentEquals(jxx::Ptr<StringBuffer> sb) const;

        jbool equalsIgnoreCase(jxx::Ptr<String> anotherString) const;
        jxx::lang::jint compareToIgnoreCase(jxx::Ptr<String> str) const;

        jbool regionMatches(jxx::lang::jint toffset, jxx::Ptr<String> other, jxx::lang::jint ooffset, jxx::lang::jint len) const;
        jbool regionMatches(jbool ignoreCase, jxx::lang::jint toffset, jxx::Ptr<String> other, jxx::lang::jint ooffset, jxx::lang::jint len) const;

        jbool startsWith(jxx::Ptr<String> prefix) const;
        jbool startsWith(jxx::Ptr<String> prefix, jxx::lang::jint toffset) const;
        jbool endsWith(jxx::Ptr<String> suffix) const;

        jxx::lang::jint indexOf(jxx::lang::jint ch) const;
        jxx::lang::jint indexOf(jxx::lang::jint ch, jxx::lang::jint fromIndex) const;
        jxx::lang::jint lastIndexOf(jxx::lang::jint ch) const;
        jxx::lang::jint lastIndexOf(jxx::lang::jint ch, jxx::lang::jint fromIndex) const;

        jxx::lang::jint indexOf(jxx::Ptr<String> str) const;
        jxx::lang::jint indexOf(jxx::Ptr<String> str, jxx::lang::jint fromIndex) const;
        jxx::lang::jint lastIndexOf(jxx::Ptr<String> str) const;
        jxx::lang::jint lastIndexOf(jxx::Ptr<String> str, jxx::lang::jint fromIndex) const;

        jxx::Ptr<String> substring(jxx::lang::jint beginIndex) const;
        jxx::Ptr<String> substring(jxx::lang::jint beginIndex, jxx::lang::jint endIndex) const;
        jxx::Ptr<String> concat(jxx::Ptr<String> str) const;

        jxx::Ptr<String> replace(jxx::lang::jchar oldChar, jxx::lang::jchar newChar) const;

        jbool matches(jxx::Ptr<String> regex) const;
        jbool contains(jxx::Ptr<CharSequence> s) const;

        jxx::Ptr<String> replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replace(jxx::Ptr<CharSequence> target, jxx::Ptr<CharSequence> replacement) const;

        jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex) const;
        jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex, jxx::lang::jint limit) const;

        jxx::Ptr<String> toLowerCase() const;
        jxx::Ptr<String> toLowerCase(jxx::Ptr<jxx::util::Locale> locale) const;
        jxx::Ptr<String> toUpperCase() const;
        jxx::Ptr<String> toUpperCase(jxx::Ptr<jxx::util::Locale> locale) const;

        jxx::Ptr<String> trim() const;

        CharArray toCharArray() const;

        // Java 8 streams:
        jxx::Ptr<jxx::util::IntStream> chars() const;
        jxx::Ptr<jxx::util::IntStream> codePoints() const;

        // Java: String intern()
        jxx::Ptr<String> intern() const;

        // ---------------------------------------------------------------------
        // Static methods (ALL Java 8 public static methods)
        // ---------------------------------------------------------------------
        static jxx::Ptr<String> valueOf(jxx::lang::jbool b);
        static jxx::Ptr<String> valueOf(jxx::lang::jchar c);
        static jxx::Ptr<String> valueOf(jxx::lang::jint i);
        static jxx::Ptr<String> valueOf(jxx::lang::jlong l);
        static jxx::Ptr<String> valueOf(jxx::lang::jfloat f);
        static jxx::Ptr<String> valueOf(jxx::lang::jdouble d);
        static jxx::Ptr<String> valueOf(jxx::Ptr<Object> obj);
        static jxx::Ptr<String> valueOf(CharArray data);
        static jxx::Ptr<String> valueOf(CharArray data, jxx::lang::jint offset, jxx::lang::jint count);

        static jxx::Ptr<String> copyValueOf(CharArray data);
        static jxx::Ptr<String> copyValueOf(CharArray data, jxx::lang::jint offset, jxx::lang::jint count);

        // Java 8 String.format(...) uses Formatter
        static jxx::Ptr<String> format(const jxx::Ptr<jxx::lang::String> formatString,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);

        static jxx::Ptr<jxx::lang::String> format(const jxx::Ptr<jxx::util::Locale> locale,
            const jxx::Ptr<jxx::lang::String> formatString,
            const jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> args);
        // Java 8 join overloads
        static jxx::Ptr<String> join(
            jxx::Ptr<CharSequence> delimiter,
            jxx::Ptr<JxxArray<jxx::Ptr<CharSequence>, 1U>> elements);
        static jxx::Ptr<String> join(
            jxx::Ptr<CharSequence> delimiter,
            jxx::Ptr<jxx::lang::Iterable<jxx::Ptr<CharSequence>>> elements);

        // ---------------------------------------------------------------------
        // Helpers used by runtime (StringBuilder/StringBuffer/etc.)
        // ---------------------------------------------------------------------
        const std::u16string& utf16() const noexcept { return value_; }
        std::string utf8() const;

        //Serializeable
        virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out);
        virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in);
        virtual void readObjectNoData();

    private:
        std::u16string value_;
        mutable jxx::lang::jint hash_ = 0;
        mutable jxx::lang::jbool hashComputed_ = false;

        // Intern pool (Java-like)
        static std::mutex internMutex_;
        static std::unordered_map<std::u16string, std::weak_ptr<String>> internPool_;

        // Conversion helpers (used for regex / UTF-8 interop)
        static std::u16string utf8ToUtf16_(const std::string& s);
        static std::string utf16ToUtf8_(const std::u16string& s);

        static inline char16_t toLowerAscii_(char16_t c);
        static inline char16_t toUpperAscii_(char16_t c);

        static inline jxx::lang::jbool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline jxx::lang::jbool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);

        static std::u16string toUtf16_(jxx::Ptr<CharSequence> s);
        static jxx::lang::jbool isTurkicLocale_(jxx::Ptr<jxx::util::Locale> loc);
    };

} // namespace jxx::lang

