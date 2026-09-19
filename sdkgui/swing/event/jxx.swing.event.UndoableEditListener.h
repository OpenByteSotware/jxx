#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class UndoableEditEvent;
    class UndoableEditListener : public ::jxx::lang::InterfaceBase<UndoableEditListener,
        ::jxx::util::EventListener>
    {
    public:
        ~UndoableEditListener() override = default;
        virtual void undoableEditHappened(
            const ::jxx::Ptr<UndoableEditEvent>& event) = 0;
    };
}
