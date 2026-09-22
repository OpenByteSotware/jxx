#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
class FieldAttributes final
    : public ::jxx::lang::ClassBase<FieldAttributes, ::jxx::lang::Object> {
public:
    FieldAttributes(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& declaredClass,
        ::jxx::lang::jint modifiers = 0);
    ::jxx::Ptr<::jxx::lang::String> getName() const;
    ::jxx::Ptr<::jxx::lang::ClassAny> getDeclaredClass() const;
    ::jxx::lang::jint getModifiers() const noexcept;
    ::jxx::lang::jbool hasModifier(::jxx::lang::jint modifier) const noexcept;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::ClassAny> declaredClass_;
    ::jxx::lang::jint modifiers_;
};
} // namespace com::google::gson
