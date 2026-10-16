#include "ext/xml/bind/jxx.ext.xml.bind.DatatypeConverter.h"

#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind {
namespace {

int hexValue(char value) noexcept {
    if (value >= '0' && value <= '9') return value - '0';
    if (value >= 'A' && value <= 'F') return value - 'A' + 10;
    if (value >= 'a' && value <= 'f') return value - 'a' + 10;
    return -1;
}

} // namespace

::jxx::Ptr<::jxx::lang::String> DatatypeConverter::printHexBinary(
    const ::jxx::lang::ByteArray& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();

    static constexpr char digits[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(static_cast<std::size_t>(value->length) * 2U);
    for (::jxx::lang::jint index = 0; index < value->length; ++index) {
        const auto byte = static_cast<unsigned char>((*value)[index]);
        result.push_back(digits[(byte >> 4U) & 0x0FU]);
        result.push_back(digits[byte & 0x0FU]);
    }
    return ::jxx::NEW<::jxx::lang::String>(result);
}

::jxx::lang::ByteArray DatatypeConverter::parseHexBinary(
    const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) {
    if (lexicalValue == nullptr) throw ::jxx::lang::NullPointerException();

    const auto text = lexicalValue->utf8();
    if ((text.size() & 1U) != 0U) {
        throw ::jxx::lang::IllegalArgumentException(
            "hex value must contain an even number of digits");
    }

    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<::jxx::lang::jint>(text.size() / 2U));
    for (std::size_t index = 0; index < text.size(); index += 2U) {
        const int high = hexValue(text[index]);
        const int low = hexValue(text[index + 1U]);
        if (high < 0 || low < 0) {
            throw ::jxx::lang::IllegalArgumentException(
                "invalid hexadecimal digit");
        }
        (*result)[static_cast<::jxx::lang::jint>(index / 2U)] =
            static_cast<::jxx::lang::jbyte>((high << 4) | low);
    }
    return result;
}

} // namespace jxx::ext::xml::bind
