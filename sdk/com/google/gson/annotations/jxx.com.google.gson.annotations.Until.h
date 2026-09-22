#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson::annotations {
class Until final : public ::jxx::lang::ClassBase<Until, ::jxx::lang::Object> {
public:
    explicit Until(::jxx::lang::jdouble value) : value_(value) {}
    ::jxx::lang::jdouble value() const noexcept { return value_; }
private: ::jxx::lang::jdouble value_;
};
} // namespace com::google::gson::annotations
