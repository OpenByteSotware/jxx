#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class ListSelectionEvent;
    class ListSelectionListener : public ::jxx::lang::InterfaceBase<
        ListSelectionListener, ::jxx::util::EventListener>
    {
    public:
        ~ListSelectionListener() override = default;
        virtual void valueChanged(const ::jxx::Ptr<ListSelectionEvent>& event) = 0;
    };
}
