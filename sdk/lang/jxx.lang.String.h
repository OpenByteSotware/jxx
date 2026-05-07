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

    class Charset;
    class Locale;
    class StringBuffer;
    class StringBuilder;
    class String;

    using StringArray = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1>;
    using StringArray2D = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 2>;
    using StringArray3D = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 3>;


    class String final : public Object, public Serializable, public Comparable<String>, public CharSequence
    {
    public:
        // ===== Constructors (Java 8) =====
        String();                                   // ""
        String(const char* utf8);                   // convenience
        String(jxx::Ptr<String> other);              // copy
        String(jxx::Ptr<CharArray> value);
        String(jxx::Ptr<CharArray> value, jint offset, jint count);

        // ===== CharSequence =====
        jint length() const override;
        jchar charAt(jint index) const override;
        jxx::Ptr<CharSequence> subSequence(jint beginIndex, jint endIndex) const override;
        //jxx::Ptr<std::string> toString() const override;

        // ===== Unicode / code points =====
        jint codePointAt(jint index) const;
        jint codePointBefore(jint index) const;
        jint codePointCount(jint beginIndex, jint endIndex) const;
        jint offsetByCodePoints(jint index, jint codePointOffset) const;

        // ===== Comparison =====
        jint compareTo(jxx::Ptr<String> anotherString) const;
        jint compareToIgnoreCase(jxx::Ptr<String> str) const;

        jbool equals(jxx::Ptr<Object> obj) const override;
        jbool equalsIgnoreCase(jxx::Ptr<String> str) const;

        // ===== Search =====
        jbool contains(jxx::Ptr<CharSequence> s) const;
        jint indexOf(jint ch) const;
        jint indexOf(jint ch, jint fromIndex) const;
        jint indexOf(jxx::Ptr<String> str) const;
        jint indexOf(jxx::Ptr<String> str, jint fromIndex) const;
        jint lastIndexOf(jint ch) const;
        jint lastIndexOf(jint ch, jint fromIndex) const;
        jint lastIndexOf(jxx::Ptr<String> str) const;
        jint lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const;

        // ===== Prefix / suffix =====
        jbool startsWith(jxx::Ptr<String> prefix) const;
        jbool startsWith(jxx::Ptr<String> prefix, jint toffset) const;
        jbool endsWith(jxx::Ptr<String> suffix) const;

        // ===== Substrings =====
        jxx::Ptr<String> substring(jint beginIndex) const;
        jxx::Ptr<String> substring(jint beginIndex, jint endIndex) const;

        // ===== Modification (returns new String) =====
        jxx::Ptr<String> concat(jxx::Ptr<String> str) const;
        jxx::Ptr<String> replace(jchar oldChar, jchar newChar) const;
        jxx::Ptr<String> trim() const;
        jxx::Ptr<String> toLowerCase() const;
        jxx::Ptr<String> toLowerCase(jxx::Ptr<Locale> locale) const;
        jxx::Ptr<String> toUpperCase() const;
        jxx::Ptr<String> toUpperCase(jxx::Ptr<Locale> locale) const;

        // ===== Arrays =====
        jxx::Ptr<CharArray> toCharArray() const;
        void getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const;

        // ===== Bytes =====
        jxx::Ptr<ByteArray> getBytes() const;
        jxx::Ptr<ByteArray> getBytes(jxx::Ptr<Charset> charset) const;
        jxx::Ptr<ByteArray> getBytes(jxx::Ptr<String> charsetName) const;

        // ===== Regex (best-effort parity) =====
        jbool matches(jxx::Ptr<String> regex) const;
        jxx::Ptr<String> replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<String> replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const;
        jxx::Ptr<StringArray> split(jxx::Ptr<String> regex) const;
        jxx::Ptr<StringArray> split(jxx::Ptr<String> regex, jint limit) const;

        // ===== Misc =====
        jbool isEmpty() const;
        jint hashCode() const override;
        jxx::Ptr<String> intern();

        static void throwNPE();
        static void throwSIOOBE();

        // ===== Static factories =====
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

        // ===== Internal helpers =====
        const std::u16string& utf16() const noexcept { return data_; }
        std::string utf8() const;   // bridge only

    private:
        std::u16string data_;
        mutable jint hash_ = 0;
        mutable jbool hashComputed_ = false;

        static std::mutex& internMutex();
        static std::unordered_map<std::u16string, std::weak_ptr<String>>& internPool();

    public:
        // ===== Unicode surrogate helpers =====
        static bool isHighSurrogate(char16_t c);
        static bool isLowSurrogate(char16_t c);
        static jint toCodePoint(char16_t high, char16_t low);
    };

} // namespace jxx::lang
