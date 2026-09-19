#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event {
class PopupMenuEvent;
class PopupMenuListener : public ::jxx::lang::InterfaceBase<PopupMenuListener, ::jxx::util::EventListener> {
public:
    ~PopupMenuListener() override = default;
    virtual void popupMenuWillBecomeVisible(const ::jxx::Ptr<PopupMenuEvent>& event) = 0;
    virtual void popupMenuWillBecomeInvisible(const ::jxx::Ptr<PopupMenuEvent>& event) = 0;
    virtual void popupMenuCanceled(const ::jxx::Ptr<PopupMenuEvent>& event) = 0;
};
}
