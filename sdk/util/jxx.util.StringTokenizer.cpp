#include "util/jxx.util.StringTokenizer.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {
namespace {
constexpr ::jxx::lang::jint highSurrogateStart = 0xD800;
constexpr ::jxx::lang::jint highSurrogateEnd = 0xDBFF;
constexpr ::jxx::lang::jint lowSurrogateStart = 0xDC00;
constexpr ::jxx::lang::jint lowSurrogateEnd = 0xDFFF;

bool isHighSurrogate(::jxx::lang::jint value) noexcept {
    return value >= highSurrogateStart && value <= highSurrogateEnd;
}

bool isLowSurrogate(::jxx::lang::jint value) noexcept {
    return value >= lowSurrogateStart && value <= lowSurrogateEnd;
}
} // namespace

StringTokenizer::StringTokenizer(
    const ::jxx::Ptr<::jxx::lang::String>& str,
    const ::jxx::Ptr<::jxx::lang::String>& delim,
    ::jxx::lang::jbool returnDelims)
    : str_(str), delimiters_(delim), returnDelimiters_(returnDelims) {
    if (str_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    maxPosition_ = str_->length();
}

StringTokenizer::StringTokenizer(
    const ::jxx::Ptr<::jxx::lang::String>& str,
    const ::jxx::Ptr<::jxx::lang::String>& delim)
    : StringTokenizer(str, delim, false) {
}

StringTokenizer::StringTokenizer(
    const ::jxx::Ptr<::jxx::lang::String>& str)
    : StringTokenizer(str, ::jxx::NEW<::jxx::lang::String>(" \t\n\r\f"), false) {
}

::jxx::lang::jint StringTokenizer::codePointAt_(
    const ::jxx::Ptr<::jxx::lang::String>& value,
    ::jxx::lang::jint index) {
    const auto first = static_cast<::jxx::lang::jint>(value->charAt(index));
    if (isHighSurrogate(first) && index + 1 < value->length()) {
        const auto second = static_cast<::jxx::lang::jint>(value->charAt(index + 1));
        if (isLowSurrogate(second)) {
            return ((first - highSurrogateStart) << 10)
                + (second - lowSurrogateStart) + 0x10000;
        }
    }
    return first;
}

::jxx::lang::jint StringTokenizer::charCount_(
    ::jxx::lang::jint codePoint) noexcept {
    return codePoint >= 0x10000 ? 2 : 1;
}

void StringTokenizer::requireDelimiters_() const {
    if (delimiters_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::lang::jbool StringTokenizer::isDelimiter_(
    ::jxx::lang::jint codePoint) const {
    requireDelimiters_();
    for (::jxx::lang::jint index = 0; index < delimiters_->length();) {
        const auto delimiter = codePointAt_(delimiters_, index);
        if (delimiter == codePoint) {
            return true;
        }
        index += charCount_(delimiter);
    }
    return false;
}

::jxx::lang::jint StringTokenizer::skipDelimiters_(
    ::jxx::lang::jint startPosition) const {
    requireDelimiters_();
    auto position = startPosition;
    while (!returnDelimiters_ && position < maxPosition_) {
        const auto codePoint = codePointAt_(str_, position);
        if (!isDelimiter_(codePoint)) {
            break;
        }
        position += charCount_(codePoint);
    }
    return position;
}

::jxx::lang::jint StringTokenizer::scanToken_(
    ::jxx::lang::jint startPosition) const {
    requireDelimiters_();
    auto position = startPosition;
    while (position < maxPosition_) {
        const auto codePoint = codePointAt_(str_, position);
        if (isDelimiter_(codePoint)) {
            break;
        }
        position += charCount_(codePoint);
    }

    if (returnDelimiters_ && position == startPosition && position < maxPosition_) {
        position += charCount_(codePointAt_(str_, position));
    }
    return position;
}

::jxx::lang::jbool StringTokenizer::hasMoreTokens() {
    return skipDelimiters_(currentPosition_) < maxPosition_;
}

::jxx::Ptr<::jxx::lang::String> StringTokenizer::nextToken() {
    currentPosition_ = skipDelimiters_(currentPosition_);
    if (currentPosition_ >= maxPosition_) {
        throw ::jxx::util::NoSuchElementException();
    }

    const auto start = currentPosition_;
    currentPosition_ = scanToken_(currentPosition_);
    return str_->substring(start, currentPosition_);
}

::jxx::Ptr<::jxx::lang::String> StringTokenizer::nextToken(
    const ::jxx::Ptr<::jxx::lang::String>& delim) {
    delimiters_ = delim;
    return nextToken();
}

::jxx::lang::jbool StringTokenizer::hasMoreElements() {
    return hasMoreTokens();
}

::jxx::Ptr<::jxx::lang::Object> StringTokenizer::nextElement() {
    return nextToken();
}

::jxx::lang::jint StringTokenizer::countTokens() {
    auto count = static_cast<::jxx::lang::jint>(0);
    auto position = currentPosition_;
    while (true) {
        position = skipDelimiters_(position);
        if (position >= maxPosition_) {
            break;
        }
        position = scanToken_(position);
        ++count;
    }
    return count;
}

} // namespace jxx::util
