#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class Object; }
namespace jxx::util { class EventObject; }
namespace jxx::swing::event { class CellEditorListener; }
namespace jxx::swing
{
    class CellEditor : public ::jxx::lang::InterfaceBase<CellEditor>
    {
    public:
        ~CellEditor() override = default;
        virtual ::jxx::Ptr<::jxx::lang::Object> getCellEditorValue() const = 0;
        virtual ::jxx::lang::jbool isCellEditable(const ::jxx::Ptr<::jxx::util::EventObject>& event) const = 0;
        virtual ::jxx::lang::jbool shouldSelectCell(const ::jxx::Ptr<::jxx::util::EventObject>& event) const = 0;
        virtual ::jxx::lang::jbool stopCellEditing() = 0;
        virtual void cancelCellEditing() = 0;
        virtual void addCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) = 0;
        virtual void removeCellEditorListener(const ::jxx::Ptr<::jxx::swing::event::CellEditorListener>& listener) = 0;
    };
}
