#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::ext::xml::bind::metadata {
class MixedContentReader : public ::jxx::lang::InterfaceBase<MixedContentReader> {
public:
    ~MixedContentReader() override = default;
    enum class Kind { TEXT, ELEMENT };
    virtual ::jxx::lang::jint size(
        const ::jxx::Ptr<::jxx::lang::Object>& source) = 0;
    virtual Kind kind(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        ::jxx::lang::jint index) = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> value(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        ::jxx::lang::jint index) = 0;
};
} // namespace jxx::ext::xml::bind::metadata
