#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class MenuShortcut : public ::jxx::lang::ClassBase<MenuShortcut,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<MenuShortcut,JxxSuper>;
 explicit MenuShortcut(::jxx::lang::jint key); MenuShortcut(::jxx::lang::jint key,::jxx::lang::jbool useShiftModifier);
 ::jxx::lang::jint getKey() const; ::jxx::lang::jbool usesShiftModifier() const;
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override; ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::lang::jint key_; ::jxx::lang::jbool usesShift_;
}; }
