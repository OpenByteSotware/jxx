#pragma once

#include "awt/event/jxx.awt.event.ActionListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing {
class Action : public ::jxx::lang::InterfaceBase<Action,
    ::jxx::awt::event::ActionListener> {
public:
    ~Action() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& key) const = 0;
    virtual void putValue(const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
    virtual void setEnabled(::jxx::lang::jbool enabled) = 0;
    virtual ::jxx::lang::jbool isEnabled() const = 0;
};
}
