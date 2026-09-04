#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include <string>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::lang {

class StringBuffer;

class StringBuilder final
    : public jxx::lang::ClassBase<StringBuilder, Object, jxx::io::SerializableI, Appendable, CharSequence> {
public:
    using JavaSuper = Object;
    using Super = jxx::lang::ClassBase<StringBuilder, Object, jxx::io::SerializableI, Appendable, CharSequence>;

public:
    StringBuilder();
    explicit StringBuilder(jint capacity);
    explicit StringBuilder(const jxx::Ptr<String>& string);
    explicit StringBuilder(const jxx::Ptr<CharSequence>& sequence);
    ~StringBuilder() override = default;

    jxx::Ptr<StringBuilder> append(jbool value);
    jxx::Ptr<Appendable> append(jchar value) override;
    jxx::Ptr<StringBuilder> appendSB(jchar value);
    jxx::Ptr<StringBuilder> append(const CharArray& value);
    jxx::Ptr<StringBuilder> append(const CharArray& value, jint offset, jint length);
    jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> value) override;
    jxx::Ptr<StringBuilder> appendSB(const jxx::Ptr<CharSequence> value);
    jxx::Ptr<Appendable> append(const jxx::Ptr<CharSequence> value, jint start, jint end) override;
    jxx::Ptr<StringBuilder> appendSB(const jxx::Ptr<CharSequence> value, jint start, jint end);
    jxx::Ptr<StringBuilder> append(jdouble value);
    jxx::Ptr<StringBuilder> append(jfloat value);
    jxx::Ptr<StringBuilder> append(jint value);
    jxx::Ptr<StringBuilder> append(jlong value);
    jxx::Ptr<StringBuilder> append(const jxx::Ptr<Object> value);
    jxx::Ptr<StringBuilder> append(const jxx::Ptr<String> value);
    jxx::Ptr<StringBuilder> append(const jxx::Ptr<StringBuffer> value);
    jxx::Ptr<StringBuilder> appendCodePoint(jint codePoint);

    jint capacity() const;
    jint length() const override;
    void ensureCapacity(jint minimumCapacity);
    void trimToSize();
    void setLength(jint newLength);

    jchar charAt(jint index) const override;
    void setCharAt(jint index, jchar value);
    jint codePointAt(jint index) const;
    jint codePointBefore(jint index) const;
    jint codePointCount(jint beginIndex, jint endIndex) const;
    jint offsetByCodePoints(jint index, jint codePointOffset) const;

    jxx::Ptr<StringBuilder> delete_(jint start, jint end);
    jxx::Ptr<StringBuilder> deleteCharAt(jint index);
    jxx::Ptr<StringBuilder> replace(jint start, jint end, const jxx::Ptr<String>& string);
    jxx::Ptr<StringBuilder> reverse();

    jxx::Ptr<StringBuilder> insert(jint offset, jbool value);
    jxx::Ptr<StringBuilder> insert(jint offset, jchar value);
    jxx::Ptr<StringBuilder> insert(jint offset, const CharArray value);
    jxx::Ptr<StringBuilder> insert(jint index, const CharArray value, jint offset, jint length);
    jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<CharSequence> value);
    jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<CharSequence> value, jint start, jint end);
    jxx::Ptr<StringBuilder> insert(jint offset, jdouble value);
    jxx::Ptr<StringBuilder> insert(jint offset, jfloat value);
    jxx::Ptr<StringBuilder> insert(jint offset, jint value);
    jxx::Ptr<StringBuilder> insert(jint offset, jlong value);
    jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<Object> value);
    jxx::Ptr<StringBuilder> insert(jint offset, const jxx::Ptr<String> value);

    void getChars(jint srcBegin, jint srcEnd, const CharArray& destination, jint dstBegin) const;
    jint indexOf(const jxx::Ptr<String>& string) const;
    jint indexOf(const jxx::Ptr<String>& string, jint fromIndex) const;
    jint lastIndexOf(const jxx::Ptr<String>& string) const;
    jint lastIndexOf(const jxx::Ptr<String>& string, jint fromIndex) const;
    jxx::Ptr<CharSequence> subSequence(jint start, jint end) const override;
    jxx::Ptr<String> substring(jint start) const;
    jxx::Ptr<String> substring(jint start, jint end) const;
    jxx::Ptr<String> toString() const override;

    void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& output) override;
    void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& input) override;
    void readObjectNoData() override;

protected:
    jxx::Ptr<Object> cloneImpl() const override;

private:
    static constexpr jint DEFAULT_CAPACITY = 16;
    std::u16string value_;
    jint capacity_;

    void ensureCapacityInternal_(jint minimumCapacity);
    jint newCapacity_(jint minimumCapacity) const;
    void appendUtf16_(const std::u16string& value);
    void insertUtf16_(jint offset, const std::u16string& value);
    static std::u16string toUtf16_(const jxx::Ptr<CharSequence>& value);
    static bool isHigh_(char16_t value) noexcept;
    static bool isLow_(char16_t value) noexcept;
    jxx::Ptr<StringBuilder> self_();
    static void throwNPE_();
    static void throwSIOOBE_();
    static void throwIAE_(const char* message);
};

} // namespace jxx::lang
