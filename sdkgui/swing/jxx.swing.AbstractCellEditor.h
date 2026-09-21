#pragma once
#include <vector>
#include "swing/jxx.swing.CellEditor.h"
namespace jxx::swing
{
    class AbstractCellEditor : public ::jxx::lang::ClassBase<AbstractCellEditor, ::jxx::lang::Object, CellEditor>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AbstractCellEditor, JxxSuper, CellEditor>;
        ~AbstractCellEditor() override = default;
        ::jxx::lang::jbool isCellEditable(const ::jxx::Ptr<::jxx::util::EventObject>& event) const override;
        ::jxx::lang::jbool shouldSelectCell(const ::jxx::Ptr<::jxx::util::EventObject>& event) const override;
        ::jxx::lang::jbool stopCellEditing() override;
        void cancelCellEditing() override;
        void addCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) override;
        void removeCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) override;
        std::vector<::jxx::Ptr<::jxx::swing::event::CellEditorListener>> getCellEditorListeners() const;
    protected:
        void fireEditingStopped();
        void fireEditingCanceled();
    private:
        std::vector<::jxx::Ptr<::jxx::swing::event::CellEditorListener>> listeners_;
    };
}
