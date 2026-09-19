#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class ChangeEvent;
    class CellEditorListener : public ::jxx::lang::InterfaceBase<CellEditorListener,
        ::jxx::util::EventListener>
    {
    public:
        ~CellEditorListener() override = default;
        virtual void editingStopped(const ::jxx::Ptr<ChangeEvent>& event) = 0;
        virtual void editingCanceled(const ::jxx::Ptr<ChangeEvent>& event) = 0;
    };
}
