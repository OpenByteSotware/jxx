#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::undo { class UndoableEdit; }
namespace jxx::swing::event
{
    class UndoableEditEvent : public ::jxx::lang::ClassBase<UndoableEditEvent,
        ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<UndoableEditEvent, JxxSuper>;
        UndoableEditEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<::jxx::swing::undo::UndoableEdit>& edit);
        ::jxx::Ptr<::jxx::swing::undo::UndoableEdit> getEdit() const;
    private:
        ::jxx::Ptr<::jxx::swing::undo::UndoableEdit> edit_;
    };
}
