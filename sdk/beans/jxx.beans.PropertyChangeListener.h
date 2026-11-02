#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.EventListener.h"
namespace jxx::beans {
class PropertyChangeEvent;
class PropertyChangeListener
    : public ::jxx::lang::InterfaceBase<PropertyChangeListener, ::jxx::util::EventListener> {
public:
    ~PropertyChangeListener() override = default;
    virtual void propertyChange(
        const ::jxx::Ptr<PropertyChangeEvent>& event) = 0;
};
} // namespace jxx::beans
