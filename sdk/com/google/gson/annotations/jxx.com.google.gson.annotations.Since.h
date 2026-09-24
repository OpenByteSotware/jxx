#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson::annotations {
class Since final : public ::jxx::lang::ClassBase<Since, ::jxx::lang::Object> {
public:
    explicit Since(::jxx::lang::jdouble value);
    ::jxx::lang::jdouble value() const noexcept;
private: ::jxx::lang::jdouble value_;
};
} // namespace com::google::gson::annotations
