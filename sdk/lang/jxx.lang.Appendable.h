#pragma once

#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class Appendable
    : public ::jxx::lang::InterfaceBase<Appendable> {
public:
    ~Appendable() override = default;

    virtual ::jxx::Ptr<Appendable> append(
        const ::jxx::Ptr<CharSequence>& sequence) = 0;

    virtual ::jxx::Ptr<Appendable> append(
        const ::jxx::Ptr<CharSequence>& sequence,
        ::jxx::lang::jint start,
        ::jxx::lang::jint end) = 0;

    virtual ::jxx::Ptr<Appendable> append(
        ::jxx::lang::jchar value) = 0;
};

} // namespace jxx::lang
