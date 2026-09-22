#include "com/google/gson/jxx.com.google.gson.FieldAttributes.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson {
FieldAttributes::FieldAttributes(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& declaredClass,
    ::jxx::lang::jint modifiers)
    : name_(name), declaredClass_(declaredClass), modifiers_(modifiers) {
    if (name_ == nullptr || declaredClass_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> FieldAttributes::getName() const { return name_; }
::jxx::Ptr<::jxx::lang::ClassAny> FieldAttributes::getDeclaredClass() const { return declaredClass_; }
::jxx::lang::jint FieldAttributes::getModifiers() const noexcept { return modifiers_; }
::jxx::lang::jbool FieldAttributes::hasModifier(::jxx::lang::jint modifier) const noexcept {
    return (modifiers_ & modifier) != 0;
}
} // namespace com::google::gson
