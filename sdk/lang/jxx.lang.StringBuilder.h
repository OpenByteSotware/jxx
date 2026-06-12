#pragma once
/*
A mutable sequence of characters.This class provides an API compatible with StringBuffer, 
but with no guarantee of synchronization.This class is designed for use as a drop - in replacement for 
StringBuffer in places where the string buffer was being used by a single thread(as is generally the case).
Where possible, it is recommended that this class be used in preference to StringBuffer as it will be faster 
under most implementations.
*/
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "jxx.lang.Appendable.h"
#include "jxx.lang.CharSequence.h"
#include "jxx.lang.String.h"

#include <string>

namespace jxx::io {
    class ObjectOutputStream;
	class ObjectInput;
    class Stream;
}

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
        explicit StringBuilder();
        explicit StringBuilder(jint capacity);
        explicit StringBuilder(const jxx::Ptr<String> str);
        explicit StringBuilder(const jxx::Ptr<CharSequence> seq);

        // ---- append overloads (Java 8) ----
        jxx::Ptr<StringBuilder> append(jbool b);
        virtual jxx::Ptr<Appendable> append(jchar c);
        jxx::Ptr<StringBuilder> appendSB(jchar c);
        jxx::Ptr<StringBuilder> append(const CharArray str);
        jxx::Ptr<StringBuilder> append(const CharArray str, jint offset, jint len);
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuilder> appendSB(const jxx::Ptr<CharSequence> s);
        virtual jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> appendSB(const jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> append(jdouble d);
        jxx::Ptr<StringBuilder> append(jfloat f);
        jxx::Ptr<StringBuilder> append(jint i);
        jxx::Ptr<StringBuilder> append(jlong lng);
        jxx::Ptr<StringBuilder> append(const jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuilder> append(const jxx::Ptr<String> str);
        jxx::Ptr<StringBuilder> append(const jxx::Ptr<StringBuffer> sb);

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
        jxx::Ptr<StringBuilder> replace(jint start, jint end, const jxx::Ptr<String> str);
        jxx::Ptr<StringBuilder> reverse();

        // ---- insert overloads ----
        jxx::Ptr<StringBuilder> insert(jint offset, jbool b);
        jxx::Ptr<StringBuilder> insert(jint offset, jchar c);
        jxx::Ptr<StringBuilder> insert(jint offset, const CharArray str);
        jxx::Ptr<StringBuilder> insert(jint index, const CharArray str, jint offset, jint len);
        jxx::Ptr<StringBuilder> insert(jint dstOffset, const jxx::Ptr<CharSequence> s);
        jxx::Ptr<StringBuilder> insert(jint dstOffset, const jxx::Ptr<CharSequence> s, jint start, jint end);
        jxx::Ptr<StringBuilder> insert(jint offset, jdouble d);
        jxx::Ptr<StringBuilder> insert(jint offset, jfloat f);
        jxx::Ptr<StringBuilder> insert(jint offset, jint i);
        jxx::Ptr<StringBuilder> insert(jint offset, jlong l);
        jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<Object> obj);
        jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<String> str);

        // ---- getChars ----
        void getChars(jint srcBegin, jint srcEnd, const CharArray dst, jint dstBegin) const;

        // ---- search ----
        jint indexOf(const jxx::Ptr<String> str) const;
        jint indexOf(const jxx::Ptr<String> str, jint fromIndex) const;
        jint lastIndexOf(const jxx::Ptr<String> str) const;
        jint lastIndexOf(const jxx::Ptr<String> str, jint fromIndex) const;

        // ---- subsequence / substring / toString ----
        jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;
        jxx::Ptr<String> substring(jint start) const;
        jxx::Ptr<String> substring(jint start, jint end) const;
        jxx::Ptr<jxx::lang::String> toString() const override;

        virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override;
        virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override;
        virtual void readObjectNoData() override;

    private:
        std::u16string value_;

        void ensureCapacityInternal_(jint minCapacity);
        jint newCapacity_(jint minCapacity) const;

        void appendUtf16_(const std::u16string& s);
        void insertUtf16_(jint offset, const std::u16string& s);
        static std::u16string toUtf16_(const jxx::Ptr<CharSequence> s);

        static inline bool isHigh_(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
        static inline bool isLow_(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

        jxx::Ptr<StringBuilder> self_();

        static void throwNPE_();
        static void throwSIOOBE_();
        static void throwIAE_(const char* msg);
    };

} // namespace jxx::lang
