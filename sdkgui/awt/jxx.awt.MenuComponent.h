#pragma once
#include "awt/jxx.awt.Font.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::awt {
class MenuComponent : public ::jxx::lang::ClassBase<MenuComponent,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<MenuComponent,JxxSuper>;
 ~MenuComponent() override=0;
 ::jxx::Ptr<Font> getFont() const; void setFont(const ::jxx::Ptr<Font>& font);
 ::jxx::Ptr<::jxx::lang::String> getName() const; void setName(const ::jxx::Ptr<::jxx::lang::String>& name);
protected: MenuComponent();
private: ::jxx::Ptr<Font> font_; ::jxx::Ptr<::jxx::lang::String> name_;
}; }
