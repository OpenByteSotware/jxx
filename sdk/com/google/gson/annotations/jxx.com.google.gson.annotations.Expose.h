#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson::annotations {
class Expose final : public ::jxx::lang::ClassBase<Expose, ::jxx::lang::Object> {
public:
    Expose(::jxx::lang::jbool serialize = true, ::jxx::lang::jbool deserialize = true)
        : serialize_(serialize), deserialize_(deserialize) {}
    ::jxx::lang::jbool serialize() const noexcept { return serialize_; }
    ::jxx::lang::jbool deserialize() const noexcept { return deserialize_; }
private:
    ::jxx::lang::jbool serialize_;
    ::jxx::lang::jbool deserialize_;
};
} // namespace com::google::gson::annotations
