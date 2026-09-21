#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class ChangeEvent;
    class ListSelectionEvent;
    class TableColumnModelEvent;

    class TableColumnModelListener : public ::jxx::lang::InterfaceBase<
        TableColumnModelListener, ::jxx::util::EventListener>
    {
    public:
        ~TableColumnModelListener() override = default;
        virtual void columnAdded(
            const ::jxx::Ptr<TableColumnModelEvent>& event) = 0;
        virtual void columnRemoved(
            const ::jxx::Ptr<TableColumnModelEvent>& event) = 0;
        virtual void columnMoved(
            const ::jxx::Ptr<TableColumnModelEvent>& event) = 0;
        virtual void columnMarginChanged(
            const ::jxx::Ptr<ChangeEvent>& event) = 0;
        virtual void columnSelectionChanged(
            const ::jxx::Ptr<ListSelectionEvent>& event) = 0;
    };
}
