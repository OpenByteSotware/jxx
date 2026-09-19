#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event {
class AncestorEvent;
class AncestorListener : public ::jxx::lang::InterfaceBase<AncestorListener, ::jxx::util::EventListener> {
public:
    ~AncestorListener() override = default;
    virtual void ancestorAdded(const ::jxx::Ptr<AncestorEvent>& event) = 0;
    virtual void ancestorRemoved(const ::jxx::Ptr<AncestorEvent>& event) = 0;
    virtual void ancestorMoved(const ::jxx::Ptr<AncestorEvent>& event) = 0;
};
}
