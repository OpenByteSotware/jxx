#include "swing/event/jxx.swing.event.UndoableEditEvent.h"
namespace jxx::swing::event
{
    UndoableEditEvent::UndoableEditEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::swing::undo::UndoableEdit>& edit)
        : Super(source), edit_(edit) {}
    ::jxx::Ptr<::jxx::swing::undo::UndoableEdit>
    UndoableEditEvent::getEdit() const { return edit_; }
}
