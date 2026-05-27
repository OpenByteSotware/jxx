#pragma once
#include <algorithm>
#include <sstream>
#include <locale>
#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "jxx.lang.Appendable.h"
#include "jxx.lang.CharSequence.h"


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
        explicit StringBuffer(jxx::lang::jint capacity);
        explicit StringBuffer(const jxx::Ptr<String> str);
        explicit StringBuffer(const jxx::Ptr<CharSequence> seq);

        // append (Java 8)
        jxx::Ptr<StringBuffer> append(jxx::lang::jbool b);
        virtual jxx::Ptr<Appendable> append(jxx::lang::jchar c);
        jxx::Ptr<StringBuffer> appendSB(jxx::lang::jchar c);
        jxx::Ptr<StringBuffer> append(const jxx::lang::CharArray str);
        jxx::Ptr<StringBuffer> append(const jxx::lang::CharArray str, jxx::lang::jint offset, jxx::lang::jint len);
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuffer> appendSB(const jxx::Ptr<CharSequence> s);
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> s, jxx::lang::jint start, jxx::lang::jint end);
        jxx::Ptr<StringBuffer> appendSB(const jxx::Ptr<CharSequence> s, jxx::lang::jint start, jxx::lang::jint end);
        jxx::Ptr<StringBuffer> append(jxx::lang::jdouble d);
        jxx::Ptr<StringBuffer> append(jxx::lang::jfloat f);
        jxx::Ptr<StringBuffer> append(jxx::lang::jint i);
        jxx::Ptr<StringBuffer> append(jxx::lang::jlong lng);
        jxx::Ptr<StringBuffer> append(const jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuffer> append(const jxx::Ptr<String> str);
        jxx::Ptr<StringBuffer> append(const jxx::Ptr<StringBuffer> sb);

        jxx::Ptr<StringBuffer> appendCodePoint(jxx::lang::jint codePoint);

        // capacity/length
        jxx::lang::jint capacity() const;
        jxx::lang::jint length() const override;
        void ensureCapacity(jxx::lang::jint minimumCapacity);
        void trimToSize();
        void setLength(jxx::lang::jint newLength);

        // char access
        jxx::lang::jchar charAt(jxx::lang::jint index) const override;
        void setCharAt(jxx::lang::jint index, jxx::lang::jchar ch);

        // code points
        jxx::lang::jint codePointAt(jxx::lang::jint index) const;
        jxx::lang::jint codePointBefore(jxx::lang::jint index) const;
        jxx::lang::jint codePointCount(jxx::lang::jint beginIndex, jxx::lang::jint endIndex) const;
        jxx::lang::jint offsetByCodePoints(jxx::lang::jint index, jxx::lang::jint codePointOffset) const;

        // delete/replace/reverse
        jxx::Ptr<StringBuffer> delete_(jxx::lang::jint start, jxx::lang::jint end);
        jxx::Ptr<StringBuffer> deleteCharAt(jxx::lang::jint index);
        jxx::Ptr<StringBuffer> replace(jxx::lang::jint start, jxx::lang::jint end, const jxx::Ptr<String> str);
        jxx::Ptr<StringBuffer> reverse();

        // insert
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jbool b);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jchar c);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, const jxx::lang::CharArray str);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint index, const jxx::lang::CharArray str, jxx::lang::jint offset, jxx::lang::jint len);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint dstOffset, const jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint dstOffset, const jxx::Ptr<CharSequence> s, jxx::lang::jint start, jxx::lang::jint end);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jdouble d);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jfloat f);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jint i);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, jxx::lang::jlong l);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, const jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuffer> insert(jxx::lang::jint offset, const jxx::Ptr<String> str);

        // getChars
        void getChars(jxx::lang::jint srcBegin, jxx::lang::jint srcEnd, const jxx::lang::CharArray dst, jxx::lang::jint dstBegin) const;

        // search
        jxx::lang::jint indexOf(const jxx::Ptr<String> str) const;
        jxx::lang::jint indexOf(const jxx::Ptr<String> str, jxx::lang::jint fromIndex) const;
        jxx::lang::jint lastIndexOf(const jxx::Ptr<String> str) const;
        jxx::lang::jint lastIndexOf(const jxx::Ptr<String> str, jxx::lang::jint fromIndex) const;

        // subsequence/substring/toString
        jxx::Ptr<CharSequence> subSequence(jxx::lang::jint start, jxx::lang::jint end) const override;
        jxx::Ptr<String> substring(jxx::lang::jint start) const;
        jxx::Ptr<String> substring(jxx::lang::jint start, jxx::lang::jint end) const;
        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        std::u16string value_;

        void ensureCapacityInternal_(jxx::lang::jint minCapacity);
        jxx::lang::jint newCapacity_(jxx::lang::jint minCapacity) const;

        void appendUtf16_(const std::u16string& s);
        void insertUtf16_(jxx::lang::jint offset, const std::u16string& s);
        static std::u16string toUtf16_(const jxx::Ptr<CharSequence> s);

        jxx::Ptr<StringBuffer> insert_(jxx::lang::jint offset, const jxx::Ptr<String> str);
        jxx::Ptr<StringBuffer> append_(const jxx::Ptr<String> str);


        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        jxx::Ptr<StringBuffer> self_();

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);
    };

} // namespace jxx::lang
