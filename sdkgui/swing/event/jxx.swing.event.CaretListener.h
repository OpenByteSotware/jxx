#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class CaretEvent;
    class CaretListener : public ::jxx::lang::InterfaceBase<CaretListener,
        ::jxx::util::EventListener>
    {
    public:
        ~CaretListener() override = default;
        virtual void caretUpdate(const ::jxx::Ptr<CaretEvent>& event) = 0;
    };
}
