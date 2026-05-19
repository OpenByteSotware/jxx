#include "jxx.lang.StringBuffer.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.StringIndexOutOfBoundsException.h"
#include "jxx.lang.IllegalArgumentException.h"

#include <algorithm>
#include <sstream>
#include <locale>

namespace jxx::lang {

    static inline std::u16string u16_from_char_array(jxx::Ptr<CharArray> a, jint off, jint len) {
        std::u16string out;
        out.resize((std::size_t)len);
        for (jint i = 0; i < len; ++i) out[(std::size_t)i] = (char16_t)(*a)[off + i];
        return out;
    }

    void StringBuffer::throwNPE_() { throw NullPointerException(JXX_NEW<String>("null")); }
    void StringBuffer::throwSIOOBE_() { throw StringIndexOutOfBoundsException(JXX_NEW<String>("String index out of range")); }
    void StringBuffer::throwIAE_(const char* msg) { throw IllegalArgumentException(JXX_NEW<String>(msg)); }

    jxx::Ptr<StringBuffer> StringBuffer::self_() {
        return this->getThis<StringBuffer>();
    }

    // ---- constructors ----
    StringBuffer::StringBuffer() : value_() {}

    StringBuffer::StringBuffer(jint capacity) : value_() {
        if (capacity < 0) throwIAE_("negative capacity");
        value_.reserve((std::size_t)capacity);
    }

    StringBuffer::StringBuffer(jxx::Ptr<String> str) : value_() {
        if (!str) throwNPE_();
        value_ = str->utf16();
    }

    StringBuffer::StringBuffer(jxx::Ptr<CharSequence> seq) : value_() {
        if (!seq) throwNPE_();
        value_ = toUtf16_(seq);
    }

    // ---- capacity / length ----
    jint StringBuffer::length() const {
        return this->synchronized([&]()->jint { return (jint)value_.size(); });
    }
    jint StringBuffer::capacity() const {
        return this->synchronized([&]()->jint { return (jint)value_.capacity(); });
    }

    jint StringBuffer::newCapacity_(jint minCapacity) const {
        jint newCap = (jint)value_.capacity() * 2 + 2;
        if (newCap < minCapacity) newCap = minCapacity;
        if (newCap < 0) newCap = minCapacity;
        return newCap;
    }

    void StringBuffer::ensureCapacityInternal_(jint minCapacity) {
        if (minCapacity <= (jint)value_.capacity()) return;
        value_.reserve((std::size_t)newCapacity_(minCapacity));
    }

    void StringBuffer::ensureCapacity(jint minimumCapacity) {
        this->synchronized([&] {
            if (minimumCapacity > 0) ensureCapacityInternal_(minimumCapacity);
            });
    }

    void StringBuffer::trimToSize() {
        this->synchronized([&] { value_.shrink_to_fit(); });
    }

    void StringBuffer::setLength(jint newLength) {
        this->synchronized([&] {
            if (newLength < 0) throwSIOOBE_();
            if (newLength <= (jint)value_.size()) value_.resize((std::size_t)newLength);
            else { ensureCapacityInternal_(newLength); value_.resize((std::size_t)newLength, (char16_t)0); }
            });
    }

    // ---- CharSequence ----
    jchar StringBuffer::charAt(jint index) const {
        return this->synchronized([&]()->jchar {
            if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
            return (jchar)value_[(std::size_t)index];
            });
    }

    void StringBuffer::setCharAt(jint index, jchar ch) {
        this->synchronized([&] {
            if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
            value_[(std::size_t)index] = (char16_t)ch;
            });
    }

    jxx::Ptr<CharSequence> StringBuffer::subSequence(jint start, jint end) const {
        return std::static_pointer_cast<CharSequence>(substring(start, end));
    }

    jxx::Ptr<jxx::lang::String> StringBuffer::toString() const {
        return this->synchronized([&]()->jxx::Ptr<jxx::lang::String> {
            auto ca = JXX_NEW<CharArray>((std::uint32_t)value_.size());
            for (jint i = 0; i < (jint)value_.size(); ++i) (*ca)[i] = (jchar)value_[(std::size_t)i];
            return JXX_NEW<String>(ca);
            });
    }

    // ---- utf16 helpers ----
    std::u16string StringBuffer::toUtf16_(jxx::Ptr<CharSequence> s) {
        if (!s) throwNPE_();
        std::u16string out;
        out.reserve((std::size_t)s->length());
        for (jint i = 0; i < s->length(); ++i) out.push_back((char16_t)s->charAt(i));
        return out;
    }

    void StringBuffer::appendUtf16_(const std::u16string& s) {
        if (s.empty()) return;
        ensureCapacityInternal_((jint)value_.size() + (jint)s.size());
        value_.append(s);
    }

    void StringBuffer::insertUtf16_(jint offset, const std::u16string& s) {
        if (offset < 0 || offset >(jint)value_.size()) throwSIOOBE_();
        if (s.empty()) return;
        ensureCapacityInternal_((jint)value_.size() + (jint)s.size());
        value_.insert(value_.begin() + offset, s.begin(), s.end());
    }

    // ---- append overloads ----
    jxx::Ptr<StringBuffer> StringBuffer::append(jbool b) { return append(JXX_NEW<String>(b ? "true" : "false")); }

    jxx::Ptr<Appendable> StringBuffer::append(jchar c) {
        this->synchronized([&] { value_.push_back((char16_t)c); });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::appendSB(jchar c) {
        this->synchronized([&] { value_.push_back((char16_t)c); });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jxx::Ptr<CharArray> str) {
        if (!str) return append(JXX_NEW<String>("null"));
        return append(str, 0, (jint)str->length);
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jxx::Ptr<CharArray> str, jint offset, jint len) {
        this->synchronized([&] {
            if (!str) { appendUtf16_(u"null"); return; }
            if (offset < 0 || len < 0 || (std::uint32_t)(offset + len) > str->length) throwSIOOBE_();
            appendUtf16_(u16_from_char_array(str, offset, len));
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::appendSB(jxx::Ptr<CharSequence> s) {
        if (!s) return append(JXX_NEW<String>("null"));
        this->synchronized([&] { appendUtf16_(toUtf16_(s)); });
        return self_();
    }

    jxx::Ptr<Appendable> StringBuffer::append(jxx::Ptr<CharSequence> s) {
        if (!s) return append(JXX_NEW<String>("null"));
        this->synchronized([&] { appendUtf16_(toUtf16_(s)); });
        return self_();
    }

    jxx::Ptr<Appendable> StringBuffer::append(jxx::Ptr<CharSequence> s, jint start, jint end) {
        this->synchronized([&] {
            if (!s) s = JXX_NEW<String>("null");
            if (start < 0 || end < start || end > s->length()) throwSIOOBE_();
            std::u16string sub;
            sub.reserve((std::size_t)(end - start));
            for (jint i = start; i < end; ++i) sub.push_back((char16_t)s->charAt(i));
            appendUtf16_(sub);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::appendSB(jxx::Ptr<CharSequence> s, jint start, jint end) {
        this->synchronized([&] {
            if (!s) s = JXX_NEW<String>("null");
            if (start < 0 || end < start || end > s->length()) throwSIOOBE_();
            std::u16string sub;
            sub.reserve((std::size_t)(end - start));
            for (jint i = start; i < end; ++i) sub.push_back((char16_t)s->charAt(i));
            appendUtf16_(sub);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jdouble d) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
        return append(JXX_NEW<String>(oss.str().c_str()));
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jfloat f) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
        return append(JXX_NEW<String>(oss.str().c_str()));
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jint i) { return append(JXX_NEW<String>(std::to_string(i).c_str())); }
    jxx::Ptr<StringBuffer> StringBuffer::append(jlong lng) { return append(JXX_NEW<String>(std::to_string((long long)lng).c_str())); }

    jxx::Ptr<StringBuffer> StringBuffer::append(jxx::Ptr<Object> obj) {
        if (!obj) return append(JXX_NEW<String>("null"));
        return append_(obj->toString());
    }

    jxx::Ptr<StringBuffer> StringBuffer::append_(jxx::Ptr<std::string> str) {
        if (!str) return append(JXX_NEW<String>("null"));
        return str->append(0, str);
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jxx::Ptr<String> str) {
        this->synchronized([&] {
            if (!str) str = JXX_NEW<String>("null");
            appendUtf16_(str->utf16());
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::append(jxx::Ptr<StringBuffer> sb) {
        if (!sb) return append(JXX_NEW<String>("null"));
        return append_(sb->toString());
    }

    // ---- appendCodePoint ----
    jxx::Ptr<StringBuffer> StringBuffer::appendCodePoint(jint codePoint) {
        this->synchronized([&] {
            if (codePoint < 0 || codePoint > 0x10FFFF) throwIAE_("Invalid code point");
            if (codePoint <= 0xFFFF) value_.push_back((char16_t)codePoint);
            else {
                jint cp = codePoint - 0x10000;
                value_.push_back((char16_t)(0xD800 + ((cp >> 10) & 0x3FF)));
                value_.push_back((char16_t)(0xDC00 + (cp & 0x3FF)));
            }
            });
        return self_();
    }

    // ---- code point queries ----
    jint StringBuffer::codePointAt(jint index) const {
        return this->synchronized([&]()->jint {
            if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
            char16_t c1 = value_[(std::size_t)index];
            if (isHigh_(c1) && (index + 1) < (jint)value_.size()) {
                char16_t c2 = value_[(std::size_t)(index + 1)];
                if (isLow_(c2)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
            }
            return (jint)c1;
            });
    }

    jint StringBuffer::codePointBefore(jint index) const {
        return this->synchronized([&]()->jint {
            if (index <= 0 || index > (jint)value_.size()) throwSIOOBE_();
            char16_t c2 = value_[(std::size_t)(index - 1)];
            if (isLow_(c2) && (index - 2) >= 0) {
                char16_t c1 = value_[(std::size_t)(index - 2)];
                if (isHigh_(c1)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
            }
            return (jint)c2;
            });
    }

    jint StringBuffer::codePointCount(jint beginIndex, jint endIndex) const {
        return this->synchronized([&]()->jint {
            if (beginIndex < 0 || endIndex < beginIndex || endIndex >(jint)value_.size()) throwSIOOBE_();
            jint count = 0;
            for (jint i = beginIndex; i < endIndex; ++i) {
                char16_t c = value_[(std::size_t)i];
                if (isHigh_(c) && (i + 1) < endIndex && isLow_(value_[(std::size_t)(i + 1)])) { ++count; ++i; }
                else ++count;
            }
            return count;
            });
    }

    jint StringBuffer::offsetByCodePoints(jint index, jint codePointOffset) const {
        return this->synchronized([&]()->jint {
            if (index < 0 || index >(jint)value_.size()) throwSIOOBE_();
            jint i = index;
            if (codePointOffset >= 0) {
                jint r = codePointOffset;
                while (r-- > 0) {
                    if (i >= (jint)value_.size()) throwSIOOBE_();
                    char16_t c = value_[(std::size_t)i++];
                    if (isHigh_(c) && i < (jint)value_.size() && isLow_(value_[(std::size_t)i])) ++i;
                }
                return i;
            }
            else {
                jint r = -codePointOffset;
                while (r-- > 0) {
                    if (i <= 0) throwSIOOBE_();
                    char16_t c2 = value_[(std::size_t)(--i)];
                    if (isLow_(c2) && i > 0 && isHigh_(value_[(std::size_t)(i - 1)])) --i;
                }
                return i;
            }
            });
    }

    // ---- delete/replace/reverse ----
    jxx::Ptr<StringBuffer> StringBuffer::delete_(jint start, jint end) {
        this->synchronized([&] {
            if (start < 0) throwSIOOBE_();
            if (end > (jint)value_.size()) end = (jint)value_.size();
            if (start > end) throwSIOOBE_();
            value_.erase(value_.begin() + start, value_.begin() + end);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::deleteCharAt(jint index) {
        this->synchronized([&] {
            if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
            value_.erase(value_.begin() + index);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::replace(jint start, jint end, jxx::Ptr<String> str) {
        this->synchronized([&] {
            if (start < 0 || start > end || end > (jint)value_.size()) throwSIOOBE_();
            if (!str) str = JXX_NEW<String>("null");
            value_.erase(value_.begin() + start, value_.begin() + end);
            insertUtf16_(start, str->utf16());
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::reverse() {
        this->synchronized([&] {
            std::reverse(value_.begin(), value_.end());
            for (std::size_t i = 0; i + 1 < value_.size(); ++i) {
                char16_t c1 = value_[i];
                char16_t c2 = value_[i + 1];
                if (isLow_(c1) && isHigh_(c2)) {
                    value_[i] = c2;
                    value_[i + 1] = c1;
                    ++i;
                }
            }
            });
        return self_();
    }

    // ---- insert ----
    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jbool b) { return insert(offset, JXX_NEW<String>(b ? "true" : "false")); }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jchar c) {
        this->synchronized([&] {
            if (offset < 0 || offset >(jint)value_.size()) throwSIOOBE_();
            value_.insert(value_.begin() + offset, (char16_t)c);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jxx::Ptr<CharArray> str) {
        if (!str) return insert(offset, JXX_NEW<String>("null"));
        return insert(offset, str, 0, (jint)str->length);
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint index, jxx::Ptr<CharArray> str, jint offset, jint len) {
        this->synchronized([&] {
            if (!str) { insertUtf16_(index, JXX_NEW<String>("null")->utf16()); return; }
            if (offset < 0 || len < 0 || (std::uint32_t)(offset + len) > str->length) throwSIOOBE_();
            insertUtf16_(index, u16_from_char_array(str, offset, len));
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint dstOffset, jxx::Ptr<CharSequence> s) {
        this->synchronized([&] {
            if (!s) s = JXX_NEW<String>("null");
            insertUtf16_(dstOffset, toUtf16_(s));
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint dstOffset, jxx::Ptr<CharSequence> s, jint start, jint end) {
        this->synchronized([&] {
            if (!s) s = JXX_NEW<String>("null");
            if (start < 0 || end < start || end > s->length()) throwSIOOBE_();
            std::u16string sub;
            sub.reserve((std::size_t)(end - start));
            for (jint i = start; i < end; ++i) sub.push_back((char16_t)s->charAt(i));
            insertUtf16_(dstOffset, sub);
            });
        return self_();
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jdouble d) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
        return insert(offset, JXX_NEW<String>(oss.str().c_str()));
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jfloat f) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
        return insert(offset, JXX_NEW<String>(oss.str().c_str()));
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jint i) { return insert(offset, JXX_NEW<String>(std::to_string(i).c_str())); }
    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jlong l) { return insert(offset, JXX_NEW<String>(std::to_string((long long)l).c_str())); }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jxx::Ptr<Object> obj) {
        if (!obj) return insert(offset, JXX_NEW<String>("null"));
        return insert_(offset, obj->toString());
    }

    jxx::Ptr<StringBuffer> StringBuffer::insert(jint offset, jxx::Ptr<String> str) {
        this->synchronized([&] {
            if (!str) str = JXX_NEW<String>("null");
            insertUtf16_(offset, str->utf16());
            });
        return self_();
    }

    // ---- getChars ----
    void StringBuffer::getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const {
        this->synchronized([&] {
            if (!dst) throwNPE_();
            if (srcBegin < 0 || srcEnd < srcBegin || srcEnd >(jint)value_.size()) throwSIOOBE_();
            if (dstBegin < 0) throwSIOOBE_();
            if ((std::uint32_t)(dstBegin + (srcEnd - srcBegin)) > dst->length) throwSIOOBE_();
            for (jint i = 0; i < (srcEnd - srcBegin); ++i) {
                (*dst)[dstBegin + i] = (jchar)value_[(std::size_t)(srcBegin + i)];
            }
            });
    }

    // ---- indexOf / lastIndexOf ----
    jint StringBuffer::indexOf(jxx::Ptr<String> str) const { return indexOf(str, 0); }

    jint StringBuffer::indexOf(jxx::Ptr<String> str, jint fromIndex) const {
        return this->synchronized([&]()->jint {
            if (!str) throwNPE_();
            if (fromIndex < 0) fromIndex = 0;
            if (fromIndex > (jint)value_.size()) return (jint)-1;
            const auto& needle = str->utf16();
            if (needle.empty()) return fromIndex;
            auto pos = value_.find(needle, (std::size_t)fromIndex);
            return (pos == std::u16string::npos) ? (jint)-1 : (jint)pos;
            });
    }

    jint StringBuffer::lastIndexOf(jxx::Ptr<String> str) const { return lastIndexOf(str, (jint)length()); }

    jint StringBuffer::lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const {
        return this->synchronized([&]()->jint {
            if (!str) throwNPE_();
            const auto& needle = str->utf16();
            if (needle.empty()) return std::min(fromIndex, (jint)value_.size());
            if (fromIndex > (jint)value_.size()) fromIndex = (jint)value_.size();
            if (fromIndex < 0) return (jint)-1;
            auto pos = value_.rfind(needle, (std::size_t)fromIndex);
            return (pos == std::u16string::npos) ? (jint)-1 : (jint)pos;
            });
    }

    // ---- substring ----
    jxx::Ptr<String> StringBuffer::substring(jint start) const { return substring(start, length()); }

    jxx::Ptr<String> StringBuffer::substring(jint start, jint end) const {
        return this->synchronized([&]()->jxx::Ptr<String> {
            if (start < 0 || end < start || end >(jint)value_.size()) throwSIOOBE_();
            jint n = end - start;
            auto ca = JXX_NEW<CharArray>((std::uint32_t)n);
            for (jint i = 0; i < n; ++i) (*ca)[i] = (jchar)value_[(std::size_t)(start + i)];
            return JXX_NEW<String>(ca);
            });
    }

} // namespace jxx::lang
