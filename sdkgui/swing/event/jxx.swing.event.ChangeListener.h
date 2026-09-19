#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class ChangeEvent;
    class ChangeListener : public ::jxx::lang::InterfaceBase<ChangeListener,
        ::jxx::util::EventListener>
    {
    public:
        ~ChangeListener() override = default;
        virtual void stateChanged(const ::jxx::Ptr<ChangeEvent>& event) = 0;
    };
}
