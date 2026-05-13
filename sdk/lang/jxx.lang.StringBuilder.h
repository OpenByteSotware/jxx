#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "jxx.lang.Appendable.h"
#include "jxx.lang.CharSequence.h"
#include "jxx.lang.String.h"

#include <string>

namespace jxx::lang {

    class StringBuffer; // for append(StringBuffer)

    class StringBuilder final
        : public Object
        , public jxx::io::Serializable
        , public Appendable
        , public CharSequence
    {
    public:
        // ---- constructors (Java 8) ----
        StringBuilder();
        explicit StringBuilder(jint capacity);
        explicit StringBuilder(jxx::Ptr<String> str);
        explicit StringBuilder(jxx::Ptr<CharSequence> seq);

        // ---- append overloads (Java 8) ----
        jxx::Ptr<StringBuilder> append(jbool b);
        virtual jxx::Ptr<Appendable> append(jchar c);
        jxx::Ptr<StringBuilder> appendSB(jchar c);
        jxx::Ptr<StringBuilder> append(jxx::Ptr<CharArray> str);
        jxx::Ptr<StringBuilder> append(jxx::Ptr<CharArray> str, jint offset, jint len);
        virtual jxx::Ptr<Appendable> append(jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuilder> appendSB(jxx::Ptr<CharSequence> s);
        virtual jxx::Ptr<Appendable> append(jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> appendSB(jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> append(jdouble d);
        jxx::Ptr<StringBuilder> append(jfloat f);
        jxx::Ptr<StringBuilder> append(jint i);
        jxx::Ptr<StringBuilder> append(jlong lng);
        jxx::Ptr<StringBuilder> append(jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuilder> append(jxx::Ptr<String> str);
        jxx::Ptr<StringBuilder> append(jxx::Ptr<StringBuffer> sb);

        jxx::Ptr<StringBuilder> appendCodePoint(jint codePoint);

        // ---- capacity / length ----
        jint capacity() const;
        jint length() const override;
        void ensureCapacity(jint minimumCapacity);
        void trimToSize();
        void setLength(jint newLength);

        // ---- char access ----
        jchar charAt(jint index) const override;
        void setCharAt(jint index, jchar ch);

        // ---- code points ----
        jint codePointAt(jint index) const;
        jint codePointBefore(jint index) const;
        jint codePointCount(jint beginIndex, jint endIndex) const;
        jint offsetByCodePoints(jint index, jint codePointOffset) const;

        // ---- delete / replace / reverse ----
        jxx::Ptr<StringBuilder> delete_(jint start, jint end);  // delete is C++ keyword
        jxx::Ptr<StringBuilder> deleteCharAt(jint index);
        jxx::Ptr<StringBuilder> replace(jint start, jint end, jxx::Ptr<String> str);
        jxx::Ptr<StringBuilder> reverse();

        // ---- insert overloads ----
        jxx::Ptr<StringBuilder> insert(jint offset, jbool b);
        jxx::Ptr<StringBuilder> insert(jint offset, jchar c);
        jxx::Ptr<StringBuilder> insert(jint offset, jxx::Ptr<CharArray> str);
        jxx::Ptr<StringBuilder> insert(jint index, jxx::Ptr<CharArray> str, jint offset, jint len);
        jxx::Ptr<StringBuilder> insert(jint dstOffset, jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuilder> insert(jint dstOffset, jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> insert(jint offset, jdouble d);
        jxx::Ptr<StringBuilder> insert(jint offset, jfloat f);
        jxx::Ptr<StringBuilder> insert(jint offset, jint i);
        jxx::Ptr<StringBuilder> insert(jint offset, jlong l);
        jxx::Ptr<StringBuilder> insert(jint offset, jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuilder> insert(jint offset, jxx::Ptr<String> str);

        // ---- getChars ----
        void getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const;

        // ---- search ----
        jint indexOf(jxx::Ptr<String> str) const;
        jint indexOf(jxx::Ptr<String> str, jint fromIndex) const;
        jint lastIndexOf(jxx::Ptr<String> str) const;
        jint lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const;

        // ---- subsequence / substring / toString ----
        jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;
        jxx::Ptr<String> substring(jint start) const;
        jxx::Ptr<String> substring(jint start, jint end) const;
        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        std::u16string value_;

        void ensureCapacityInternal_(jint minCapacity);
        jint newCapacity_(jint minCapacity) const;

        void appendUtf16_(const std::u16string& s);
        void insertUtf16_(jint offset, const std::u16string& s);
        static std::u16string toUtf16_(jxx::Ptr<CharSequence> s);

        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        jxx::Ptr<StringBuilder> self_();

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);
    };

} // namespace jxx::lang
