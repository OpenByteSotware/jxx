#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include "io/jxx.io.ModifiedUTF.h"
#include "io/jxx.io.UTFDataFormatException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

namespace {

bool isContinuation(std::uint8_t value) {
    return (value & 0xc0U) == 0x80U;
}

} // namespace

::jxx::lang::ByteArray ModifiedUTF::encode(
    const ::jxx::Ptr<::jxx::lang::String>& value) {

    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    std::size_t encodedLength = 0;
    for (::jxx::lang::jint index = 0;
         index < value->length();
         ++index) {
        const auto character =
            static_cast<std::uint16_t>(value->charAt(index));
        if (character >= 0x0001U && character <= 0x007fU) {
            ++encodedLength;
        }
        else if (character <= 0x07ffU) {
            encodedLength += 2;
        }
        else {
            encodedLength += 3;
        }

        if (encodedLength >
            static_cast<std::size_t>(
                std::numeric_limits<std::uint16_t>::max())) {
            throw UTFDataFormatException();
        }
    }

    const auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(encodedLength));
    std::size_t output = 0;

    for (::jxx::lang::jint index = 0;
         index < value->length();
         ++index) {
        const auto character =
            static_cast<std::uint16_t>(value->charAt(index));

        if (character >= 0x0001U && character <= 0x007fU) {
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(character);
        }
        else if (character <= 0x07ffU) {
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(
                    0xc0U | ((character >> 6U) & 0x1fU));
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(
                    0x80U | (character & 0x3fU));
        }
        else {
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(
                    0xe0U | ((character >> 12U) & 0x0fU));
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(
                    0x80U | ((character >> 6U) & 0x3fU));
            (*result)[static_cast<::jxx::lang::jint>(output++)] =
                static_cast<::jxx::lang::jbyte>(
                    0x80U | (character & 0x3fU));
        }
    }

    return result;
}

::jxx::Ptr<::jxx::lang::String> ModifiedUTF::decode(
    const ::jxx::lang::ByteArray& bytes) {

    if (bytes == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    std::u16string value;
    value.reserve(bytes->length);
    std::size_t index = 0;

    while (index < bytes->length) {
        const auto first = static_cast<std::uint8_t>(
            (*bytes)[static_cast<::jxx::lang::jint>(index++)]);

        if (first >= 0x01U && first <= 0x7fU) {
            value.push_back(static_cast<char16_t>(first));
            continue;
        }

        if ((first & 0xe0U) == 0xc0U) {
            if (index >= bytes->length) {
                throw UTFDataFormatException();
            }
            const auto second = static_cast<std::uint8_t>(
                (*bytes)[static_cast<::jxx::lang::jint>(index++)]);
            if (!isContinuation(second)) {
                throw UTFDataFormatException();
            }

            const auto character = static_cast<std::uint16_t>(
                ((first & 0x1fU) << 6U) | (second & 0x3fU));
            if (character == 0U) {
                if (first != 0xc0U || second != 0x80U) {
                    throw UTFDataFormatException();
                }
            }
            else if (character < 0x0080U) {
                throw UTFDataFormatException();
            }
            value.push_back(static_cast<char16_t>(character));
            continue;
        }

        if ((first & 0xf0U) == 0xe0U) {
            if (index + 1U >= bytes->length) {
                throw UTFDataFormatException();
            }
            const auto second = static_cast<std::uint8_t>(
                (*bytes)[static_cast<::jxx::lang::jint>(index++)]);
            const auto third = static_cast<std::uint8_t>(
                (*bytes)[static_cast<::jxx::lang::jint>(index++)]);
            if (!isContinuation(second) || !isContinuation(third)) {
                throw UTFDataFormatException();
            }

            const auto character = static_cast<std::uint16_t>(
                ((first & 0x0fU) << 12U) |
                ((second & 0x3fU) << 6U) |
                (third & 0x3fU));
            if (character < 0x0800U) {
                throw UTFDataFormatException();
            }
            value.push_back(static_cast<char16_t>(character));
            continue;
        }

        throw UTFDataFormatException();
    }

    return ::jxx::NEW<::jxx::lang::String>(value);
}

} // namespace jxx::io
