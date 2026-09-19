#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event {
class ListDataEvent;
class ListDataListener : public ::jxx::lang::InterfaceBase<ListDataListener, ::jxx::util::EventListener> {
public:
    ~ListDataListener() override = default;
    virtual void intervalAdded(const ::jxx::Ptr<ListDataEvent>& event) = 0;
    virtual void intervalRemoved(const ::jxx::Ptr<ListDataEvent>& event) = 0;
    virtual void contentsChanged(const ::jxx::Ptr<ListDataEvent>& event) = 0;
};
}
