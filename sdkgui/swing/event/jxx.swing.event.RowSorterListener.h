#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class RowSorterEvent;

    class RowSorterListener : public ::jxx::lang::InterfaceBase<
        RowSorterListener, ::jxx::util::EventListener>
    {
    public:
        ~RowSorterListener() override = default;
        virtual void sorterChanged(
            const ::jxx::Ptr<RowSorterEvent>& event) = 0;
    };
}
