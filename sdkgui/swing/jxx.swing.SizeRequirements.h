#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::swing {
class SizeRequirements final : public ::jxx::lang::ClassBase<SizeRequirements, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SizeRequirements, JxxSuper>;

    ::jxx::lang::jint minimum = 0;
    ::jxx::lang::jint preferred = 0;
    ::jxx::lang::jint maximum = 0;
    ::jxx::lang::jfloat alignment = 0.5F;

    SizeRequirements() = default;
    SizeRequirements(::jxx::lang::jint minimum,
        ::jxx::lang::jint preferred,
        ::jxx::lang::jint maximum,
        ::jxx::lang::jfloat alignment);

    static ::jxx::Ptr<SizeRequirements> getTiledSizeRequirements(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<SizeRequirements>, 1>>& children);
    static ::jxx::Ptr<SizeRequirements> getAlignedSizeRequirements(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<SizeRequirements>, 1>>& children);
};
}
