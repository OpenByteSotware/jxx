#pragma once
#include "awt/jxx.awt.Component.h"
#include "lang/jxx.lang.String.h"
namespace jxx::awt {
class Label : public ::jxx::lang::ClassBase<Label, Component> {
public:
 using JxxSuper=Component; using Super=::jxx::lang::ClassBase<Label,JxxSuper>;
 static constexpr ::jxx::lang::jint LEFT=0,CENTER=1,RIGHT=2;
 Label(); explicit Label(const ::jxx::Ptr<::jxx::lang::String>& text); Label(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint alignment);
 ~Label() override=default;
 ::jxx::Ptr<::jxx::lang::String> getText() const; void setText(const ::jxx::Ptr<::jxx::lang::String>& text);
 ::jxx::lang::jint getAlignment() const; void setAlignment(::jxx::lang::jint alignment);
private: ::jxx::Ptr<::jxx::lang::String> text_; ::jxx::lang::jint alignment_;
}; }
