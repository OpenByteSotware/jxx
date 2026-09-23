#include <algorithm>
#include <cstddef>
#include "io/jxx.io.UTF8.h"

namespace jxx::io {

namespace {

constexpr std::uint32_t replacement = 0xfffdU;

void appendEncoded(
    std::vector<std::uint8_t>& output,
    std::uint32_t codePoint) {

    if (codePoint <= 0x7fU) {
        output.push_back(static_cast<std::uint8_t>(codePoint));
    }
    else if (codePoint <= 0x7ffU) {
        output.push_back(static_cast<std::uint8_t>(
            0xc0U | ((codePoint >> 6U) & 0x1fU)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | (codePoint & 0x3fU)));
    }
    else if (codePoint <= 0xffffU) {
        output.push_back(static_cast<std::uint8_t>(
            0xe0U | ((codePoint >> 12U) & 0x0fU)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | ((codePoint >> 6U) & 0x3fU)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | (codePoint & 0x3fU)));
    }
    else {
        output.push_back(static_cast<std::uint8_t>(
            0xf0U | ((codePoint >> 18U) & 0x07U)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | ((codePoint >> 12U) & 0x3fU)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | ((codePoint >> 6U) & 0x3fU)));
        output.push_back(static_cast<std::uint8_t>(
            0x80U | (codePoint & 0x3fU)));
    }
}

bool isContinuation(std::uint8_t value) {
    return (value & 0xc0U) == 0x80U;
}

void appendDecoded(
    std::u16string& output,
    std::uint32_t codePoint) {

    if (codePoint <= 0xffffU) {
        output.push_back(static_cast<char16_t>(codePoint));
        return;
    }

    codePoint -= 0x10000U;
    output.push_back(static_cast<char16_t>(
        0xd800U | ((codePoint >> 10U) & 0x3ffU)));
    output.push_back(static_cast<char16_t>(
        0xdc00U | (codePoint & 0x3ffU)));
}

} // namespace

::jxx::lang::ByteArray UTF8::encode(const std::u16string& value) {
    std::vector<std::uint8_t> output;
    output.reserve(value.size() * 2U);

    std::size_t index = 0;
    while (index < value.size()) {
        const auto first = static_cast<std::uint16_t>(value[index++]);

        if (first >= 0xd800U && first <= 0xdbffU) {
            if (index < value.size()) {
                const auto second =
                    static_cast<std::uint16_t>(value[index]);
                if (second >= 0xdc00U && second <= 0xdfffU) {
                    ++index;
                    appendEncoded(
                        output,
                        0x10000U +
                            ((static_cast<std::uint32_t>(first - 0xd800U)
                              << 10U) |
                             static_cast<std::uint32_t>(
                                 second - 0xdc00U)));
                    continue;
                }
            }
            appendEncoded(output, replacement);
        }
        else if (first >= 0xdc00U && first <= 0xdfffU) {
            appendEncoded(output, replacement);
        }
        else {
            appendEncoded(output, first);
        }
    }

    const auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(output.size()));
    std::copy(output.begin(), output.end(), result->data());
    return result;
}

std::u16string UTF8::decode(const ::jxx::lang::ByteArray bytes) {
    std::u16string output;
    if (bytes == nullptr) {
        return output;
    }

    std::size_t index = 0;
    while (index < bytes->length) {
        const auto first = static_cast<std::uint8_t>(
            (*bytes)[static_cast<::jxx::lang::jint>(index)]);

        std::uint32_t codePoint = replacement;
        std::size_t sequenceLength = 1;

        if (first <= 0x7fU) {
            codePoint = first;
        }
        else if (first >= 0xc2U && first <= 0xdfU) {
            if (index + 1U < bytes->length) {
                const auto second = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 1U)]);
                if (isContinuation(second)) {
                    codePoint =
                        ((first & 0x1fU) << 6U) | (second & 0x3fU);
                    sequenceLength = 2;
                }
            }
        }
        else if (first >= 0xe0U && first <= 0xefU) {
            if (index + 2U < bytes->length) {
                const auto second = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 1U)]);
                const auto third = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 2U)]);
                const bool validSecond =
                    isContinuation(second) &&
                    !(first == 0xe0U && second < 0xa0U) &&
                    !(first == 0xedU && second >= 0xa0U);
                if (validSecond && isContinuation(third)) {
                    codePoint =
                        ((first & 0x0fU) << 12U) |
                        ((second & 0x3fU) << 6U) |
                        (third & 0x3fU);
                    sequenceLength = 3;
                }
            }
        }
        else if (first >= 0xf0U && first <= 0xf4U) {
            if (index + 3U < bytes->length) {
                const auto second = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 1U)]);
                const auto third = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 2U)]);
                const auto fourth = static_cast<std::uint8_t>(
                    (*bytes)[static_cast<::jxx::lang::jint>(index + 3U)]);
                const bool validSecond =
                    isContinuation(second) &&
                    !(first == 0xf0U && second < 0x90U) &&
                    !(first == 0xf4U && second > 0x8fU);
                if (validSecond &&
                    isContinuation(third) &&
                    isContinuation(fourth)) {
                    codePoint =
                        ((first & 0x07U) << 18U) |
                        ((second & 0x3fU) << 12U) |
                        ((third & 0x3fU) << 6U) |
                        (fourth & 0x3fU);
                    sequenceLength = 4;
                }
            }
        }

        appendDecoded(output, codePoint);
        index += sequenceLength;
    }

    return output;
}

} // namespace jxx::io
