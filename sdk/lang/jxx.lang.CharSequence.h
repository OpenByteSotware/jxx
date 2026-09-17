#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class String;

class CharSequence
    : public ::jxx::lang::InterfaceBase<CharSequence> {
public:
    ~CharSequence() override = default;

    virtual ::jxx::lang::jint length() const = 0;

    virtual ::jxx::lang::jchar charAt(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<CharSequence> subSequence(
        ::jxx::lang::jint start,
        ::jxx::lang::jint end) const = 0;

    virtual ::jxx::Ptr<String> toString() const = 0;
};

} // namespace jxx::lang
