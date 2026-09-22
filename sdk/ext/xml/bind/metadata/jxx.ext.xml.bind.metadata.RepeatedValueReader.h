#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class RepeatedValueReader
    : public ::jxx::lang::InterfaceBase<RepeatedValueReader> {
public:
    ~RepeatedValueReader() override = default;

    virtual ::jxx::lang::jint size(
        const ::jxx::Ptr<::jxx::lang::Object>& collection) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> get(
        const ::jxx::Ptr<::jxx::lang::Object>& collection,
        ::jxx::lang::jint index) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
