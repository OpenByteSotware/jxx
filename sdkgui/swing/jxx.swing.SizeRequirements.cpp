#include "swing/jxx.swing.SizeRequirements.h"

#include <algorithm>
#include <cstdint>
#include <limits>

namespace jxx::swing {
namespace {
::jxx::lang::jint saturatedAdd(::jxx::lang::jint left, ::jxx::lang::jint right) {
    const auto value = static_cast<std::int64_t>(left) + right;
    return static_cast<::jxx::lang::jint>(std::min<std::int64_t>(
        value, std::numeric_limits<::jxx::lang::jint>::max()));
}
}
SizeRequirements::SizeRequirements(::jxx::lang::jint minimumValue,
    ::jxx::lang::jint preferredValue, ::jxx::lang::jint maximumValue,
    ::jxx::lang::jfloat alignmentValue)
    : minimum(minimumValue), preferred(preferredValue), maximum(maximumValue),
      alignment(alignmentValue) {}

::jxx::Ptr<SizeRequirements> SizeRequirements::getTiledSizeRequirements(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<SizeRequirements>, 1>>& children) {
    auto result = ::jxx::NEW<SizeRequirements>();
    if (children == nullptr) return result;
    for (std::uint32_t index = 0; index < children->length; ++index) {
        const auto& child = (*children)[index];
        if (child == nullptr) continue;
        result->minimum = saturatedAdd(result->minimum, child->minimum);
        result->preferred = saturatedAdd(result->preferred, child->preferred);
        result->maximum = saturatedAdd(result->maximum, child->maximum);
    }
    return result;
}

::jxx::Ptr<SizeRequirements> SizeRequirements::getAlignedSizeRequirements(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<SizeRequirements>, 1>>& children) {
    auto result = ::jxx::NEW<SizeRequirements>();
    if (children == nullptr) return result;
    for (std::uint32_t index = 0; index < children->length; ++index) {
        const auto& child = (*children)[index];
        if (child == nullptr) continue;
        result->minimum = std::max(result->minimum, child->minimum);
        result->preferred = std::max(result->preferred, child->preferred);
        result->maximum = std::max(result->maximum, child->maximum);
    }
    return result;
}
}
