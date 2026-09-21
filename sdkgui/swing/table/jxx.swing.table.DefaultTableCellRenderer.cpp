#include "swing/table/jxx.swing.table.DefaultTableCellRenderer.h"

#include "swing/jxx.swing.JTable.h"

namespace jxx::swing::table
{
    DefaultTableCellRenderer::DefaultTableCellRenderer() : Super()
    {
        setHorizontalAlignment(::jxx::swing::SwingConstants::LEADING);
    }

    void DefaultTableCellRenderer::setValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        setText(value == nullptr
            ? ::jxx::NEW<::jxx::lang::String>("")
            : value->toString());
    }

    ::jxx::Ptr<::jxx::awt::Component>
    DefaultTableCellRenderer::getTableCellRendererComponent(
        const ::jxx::Ptr<::jxx::swing::JTable>& table,
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        ::jxx::lang::jbool,
        ::jxx::lang::jbool,
        ::jxx::lang::jint,
        ::jxx::lang::jint)
    {
        if (table != nullptr)
        {
            setFont(table->getFont());
            setForeground(table->getForeground());
            setBackground(table->getBackground());
        }
        setValue(value);
        return ::jxx::CAST<::jxx::awt::Component>(thisPtr());
    }

    void DefaultTableCellRenderer::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void DefaultTableCellRenderer::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
    }

    void DefaultTableCellRenderer::readObjectNoData()
    {
        setText(::jxx::NEW<::jxx::lang::String>(""));
    }
}
