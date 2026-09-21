#include "swing/jxx.swing.AbstractCellEditor.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.CellEditorListener.h"
#include "swing/event/jxx.swing.event.ChangeEvent.h"
namespace jxx::swing
{
    ::jxx::lang::jbool AbstractCellEditor::isCellEditable(const ::jxx::Ptr<::jxx::util::EventObject>&) const { return true; }
    ::jxx::lang::jbool AbstractCellEditor::shouldSelectCell(const ::jxx::Ptr<::jxx::util::EventObject>&) const { return true; }
    ::jxx::lang::jbool AbstractCellEditor::stopCellEditing() { fireEditingStopped(); return true; }
    void AbstractCellEditor::cancelCellEditing() { fireEditingCanceled(); }
    void AbstractCellEditor::addCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) { if(listener) listeners_.push_back(listener); }
    void AbstractCellEditor::removeCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) { listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),listener),listeners_.end()); }
    std::vector<::jxx::Ptr<::jxx::swing::event::CellEditorListener>> AbstractCellEditor::getCellEditorListeners() const { return listeners_; }
    void AbstractCellEditor::fireEditingStopped() { auto event=::jxx::NEW<::jxx::swing::event::ChangeEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr())); const auto copy=listeners_; for(const auto& listener:copy) if(listener) listener->editingStopped(event); }
    void AbstractCellEditor::fireEditingCanceled() { auto event=::jxx::NEW<::jxx::swing::event::ChangeEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr())); const auto copy=listeners_; for(const auto& listener:copy) if(listener) listener->editingCanceled(event); }
}
