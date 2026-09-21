#include "swing/jxx.swing.DefaultListCellRenderer.h"

#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.JList.h"

namespace jxx::swing
{
    DefaultListCellRenderer::DefaultListCellRenderer()
        : Super(::jxx::NEW<::jxx::lang::String>(""))
    {
        setOpaque(true);
    }

    ::jxx::Ptr<::jxx::awt::Component>
    DefaultListCellRenderer::getListCellRendererComponent(
        const ::jxx::Ptr<JList<::jxx::lang::Object>>& list,
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        ::jxx::lang::jint,
        ::jxx::lang::jbool isSelected,
        ::jxx::lang::jbool)
    {
        setText(value == nullptr
            ? ::jxx::NEW<::jxx::lang::String>("")
            : value->toString());

        if (list != nullptr)
        {
            setEnabled(list->isEnabled());
            setFont(list->getFont());
            if (!isSelected)
            {
                setForeground(list->getForeground());
                setBackground(list->getBackground());
            }
        }

        return ::jxx::CAST<::jxx::awt::Component>(thisPtr());
    }
}
