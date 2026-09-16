#pragma once
#include "awt/jxx.awt.AWTEvent.h"
namespace jxx::awt::event { class TextEvent : public ::jxx::lang::ClassBase<TextEvent,::jxx::awt::AWTEvent>{public:using JxxSuper=::jxx::awt::AWTEvent;using Super=::jxx::lang::ClassBase<TextEvent,JxxSuper>;static constexpr ::jxx::lang::jint TEXT_FIRST=900;static constexpr ::jxx::lang::jint TEXT_VALUE_CHANGED=900;static constexpr ::jxx::lang::jint TEXT_LAST=900;TextEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id);protected: ::jxx::Ptr<::jxx::lang::String> paramString() const override;}; }
