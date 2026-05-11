#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.io.Serializable.h"
#include "jxx.lang.Appendable.h"
#include "jxx.lang.CharSequence.h"
#include "jxx.lang.String.h"

#include <string>

namespace jxx::lang {

    class StringBuffer final
        : public Object
        , public jxx::io::Serializable
        , public Appendable
        , public CharSequence
    {
    public:
        StringBuffer();
        explicit StringBuffer(jint capacity);
        explicit StringBuffer(jxx::Ptr<String> str);
        explicit StringBuffer(jxx::Ptr<CharSequence> seq);

        // append (Java 8)
        jxx::Ptr<StringBuffer> append(jbool b);
        jxx::Ptr<Appendable> append(jchar c);
        jxx::Ptr<StringBuffer> appendSB(jchar c);
        jxx::Ptr<StringBuffer> append(jxx::Ptr<CharArray> str);
        jxx::Ptr<StringBuffer> append(jxx::Ptr<CharArray> str, jint offset, jint len);
        jxx::Ptr<Appendable> append(jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuffer> appendSB(jxx::Ptr<CharSequence> s);
        jxx::Ptr<Appendable> append(jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuffer> appendSB(jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuffer> append(jdouble d);
        jxx::Ptr<StringBuffer> append(jfloat f);
        jxx::Ptr<StringBuffer> append(jint i);
        jxx::Ptr<StringBuffer> append(jlong lng);
        jxx::Ptr<StringBuffer> append(jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuffer> append(jxx::Ptr<String> str);
        jxx::Ptr<StringBuffer> append(jxx::Ptr<StringBuffer> sb);

        jxx::Ptr<StringBuffer> appendCodePoint(jint codePoint);

        // capacity/length
        jint capacity() const;
        jint length() const override;
        void ensureCapacity(jint minimumCapacity);
        void trimToSize();
        void setLength(jint newLength);

        // char access
        jchar charAt(jint index) const override;
        void setCharAt(jint index, jchar ch);

        // code points
        jint codePointAt(jint index) const;
        jint codePointBefore(jint index) const;
        jint codePointCount(jint beginIndex, jint endIndex) const;
        jint offsetByCodePoints(jint index, jint codePointOffset) const;

        // delete/replace/reverse
        jxx::Ptr<StringBuffer> delete_(jint start, jint end);
        jxx::Ptr<StringBuffer> deleteCharAt(jint index);
        jxx::Ptr<StringBuffer> replace(jint start, jint end, jxx::Ptr<String> str);
        jxx::Ptr<StringBuffer> reverse();

        // insert
        jxx::Ptr<StringBuffer> insert(jint offset, jbool b);
        jxx::Ptr<StringBuffer> insert(jint offset, jchar c);
        jxx::Ptr<StringBuffer> insert(jint offset, jxx::Ptr<CharArray> str);
        jxx::Ptr<StringBuffer> insert(jint index, jxx::Ptr<CharArray> str, jint offset, jint len);
        jxx::Ptr<StringBuffer> insert(jint dstOffset, jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuffer> insert(jint dstOffset, jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuffer> insert(jint offset, jdouble d);
        jxx::Ptr<StringBuffer> insert(jint offset, jfloat f);
        jxx::Ptr<StringBuffer> insert(jint offset, jint i);
        jxx::Ptr<StringBuffer> insert(jint offset, jlong l);
        jxx::Ptr<StringBuffer> insert(jint offset, jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuffer> insert(jint offset, jxx::Ptr<String> str);

        // getChars
        void getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const;

        // search
        jint indexOf(jxx::Ptr<String> str) const;
        jint indexOf(jxx::Ptr<String> str, jint fromIndex) const;
        jint lastIndexOf(jxx::Ptr<String> str) const;
        jint lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const;

        // subsequence/substring/toString
        jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;
        jxx::Ptr<String> substring(jint start) const;
        jxx::Ptr<String> substring(jint start, jint end) const;
        jxx::Ptr<std::string> toString() const override;

    private:
        std::u16string value_;

        void ensureCapacityInternal_(jint minCapacity);
        jint newCapacity_(jint minCapacity) const;

        void appendUtf16_(const std::u16string& s);
        void insertUtf16_(jint offset, const std::u16string& s);
        static std::u16string toUtf16_(jxx::Ptr<CharSequence> s);

        jxx::Ptr<StringBuffer> insert_(jint offset, jxx::Ptr<std::string> str);
        jxx::Ptr<StringBuffer> append_(jxx::Ptr<std::string> str);


        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        jxx::Ptr<StringBuffer> self_();

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);
    };

} // namespace jxx::lang
