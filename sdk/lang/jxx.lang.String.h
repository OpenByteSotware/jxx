#pragma once

#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.CharSequence.h"
#include "jxx.lang.Comparable.h"
#include "io/jxx.io.Serializable.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

namespace jxx::lang {

    // Forward declarations for strict API completeness
    class StringBuffer;
    class StringBuilder;
    class Charset; // java.nio.charset.Charset (optional)
    class Locale;  // java.util.Locale (optional)

} // namespace jxx::lang

namespace jxx::util { class IntStream; }

namespace jxx::lang {

    /**
     * Strict Java 8 parity java.lang.String (C++17).
     *
     * Implements:
     *  - Object
     *  - CharSequence
     *  - Comparable<String>
     *  - Serializable
     *
     * All reference-type APIs use jxx::Ptr<T>.
     */
    class String final
        : public Object
        , public CharSequence
        , public Comparable<String>
        , public jxx::io::Serializable
    {
    public:
        // ---------------------------------------------------------------------
        // Constructors (Java 8 public constructors)
        // ---------------------------------------------------------------------
        String();                                      // ""
        String(jxx::Ptr<String> original);             // copy

        // byte[] ctors
        explicit String(jxx::Ptr<ByteArray> bytes);
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length);
        String(jxx::Ptr<ByteArray> bytes, jxx::Ptr<String> charsetName);
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length, jxx::Ptr<String> charsetName);
        String(jxx::Ptr<ByteArray> bytes, jxx::Ptr<Charset> charset);
        String(jxx::Ptr<ByteArray> bytes, jint offset, jint length, jxx::Ptr<Charset> charset);

        // deprecated byte[] hibyte ctors
        String(jxx::Ptr<ByteArray> ascii, jint hibyte);
        String(jxx::Ptr<ByteArray> ascii, jint hibyte, jint offset, jint count);

        // char[] ctors
        explicit String(jxx::Ptr<CharArray> value);
        String(jxx::Ptr<CharArray> value, jint offset, jint count);

        // codePoints ctor
        String(jxx::Ptr<IntArray> codePoints, jint offset, jint count);

        // StringBuffer / StringBuilder ctors
        explicit String(jxx::Ptr<StringBuffer> buffer);
        explicit String(jxx::Ptr<StringBuilder> builder);

        // JXX convenience ctor (UTF-8 c-string)
        explicit String(const char* utf8);

        // ---------------------------------------------------------------------
        // CharSequence
        // ---------------------------------------------------------------------
        jint length() const override;
        jchar charAt(jint index) const override;
        jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;
        jxx::Ptr<jxx::lang::String> toString() const override;

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
        // Java 8 instance methods (strict completeness)
        // ---------------------------------------------------------------------
        jbool isEmpty() const;

        jint codePointAt(jint index) const;
        jint codePointBefore(jint index) const;
        jint codePointCount(jint beginIndex, jint endIndex) const;
        jint offsetByCodePoints(jint index, jint codePointOffset) const;

        void getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const;

        // deprecated Java 8:
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

        jxx::Ptr<CharSequence> subSequence_(jint start, jint end) const; // alias if needed
        jxx::Ptr<String> concat(jxx::Ptr<String> str) const;

        jxx::Ptr<String> replace(jchar oldChar, jchar newChar) const;
        jbool matches(jxx::Ptr<String> regex) const;
        jbool contains(jxx::Ptr<CharSequence> s) const;

        jxx::Ptr<String> replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replace(jxx::Ptr<CharSequence> target, jxx::Ptr<CharSequence> replacement) const;

        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex) const;
        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<String>, 1>> split(jxx::Ptr<String> regex, jint limit) const;

        jxx::Ptr<String> toLowerCase() const;
        jxx::Ptr<String> toLowerCase(jxx::Ptr<Locale> locale) const;
        jxx::Ptr<String> toUpperCase() const;
        jxx::Ptr<String> toUpperCase(jxx::Ptr<Locale> locale) const;

        jxx::Ptr<String> trim() const;

        jxx::Ptr<CharArray> toCharArray() const;

        // Java 8 streams
        jxx::Ptr<jxx::util::IntStream> chars() const;
        jxx::Ptr<jxx::util::IntStream> codePoints() const;

        // native in Java; here we implement
        jxx::Ptr<String> intern() const;

        // ---------------------------------------------------------------------
        // Java 8 static methods (strict completeness)
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

        // format (strict signature; delegate to your jxx.util.Formatter if present)
        static jxx::Ptr<String> format(jxx::Ptr<String> format,
            jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Object>, 1>> args);

        static jxx::Ptr<String> format(jxx::Ptr<Locale> l, jxx::Ptr<String> format,
            jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Object>, 1>> args);

        // join overloads (Java 8)
        static jxx::Ptr<String> join(jxx::Ptr<CharSequence> delimiter,
            jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<CharSequence>, 1>> elements);

        // Iterable overload (forward-declared template in your util package if you have it)
        template <class IterableCharSeq>
        static jxx::Ptr<String> joinIterable(jxx::Ptr<CharSequence> delimiter, jxx::Ptr<IterableCharSeq> elements);

        // ---------------------------------------------------------------------
        // JXX helpers
        // ---------------------------------------------------------------------
        const std::u16string& utf16() const noexcept { return value_; }
        std::string utf8() const;

    private:
        std::u16string value_;
        mutable jint hash_ = 0;
        mutable jbool hashComputed_ = false;

        // intern pool
        static std::mutex internMutex_;
        static std::unordered_map<std::u16string, std::weak_ptr<String>> internPool_;

        // conversion
        static std::u16string utf8ToUtf16_(const std::string& s);
        static std::string utf16ToUtf8_(const std::u16string& s);

        static inline char16_t toLowerAscii_(char16_t c);
        static inline char16_t toUpperAscii_(char16_t c);

        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        static void throwNPE_();
        static void throwSIOOBE_();

        static std::u16string toUtf16_(jxx::Ptr<CharSequence> s);
    };

} 
