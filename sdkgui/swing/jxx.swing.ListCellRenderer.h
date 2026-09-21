#pragma once
#include "awt/jxx.awt.Component.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    template<typename E>
    class JList;

    template<typename E>
    class ListCellRenderer : public ::jxx::lang::InterfaceBase<ListCellRenderer<E>>
    {
    public:
        ~ListCellRenderer() override = default;

        virtual ::jxx::Ptr<::jxx::awt::Component> getListCellRendererComponent(
            const ::jxx::Ptr<JList<E>>& list,
            const ::jxx::Ptr<E>& value,
            ::jxx::lang::jint index,
            ::jxx::lang::jbool isSelected,
            ::jxx::lang::jbool cellHasFocus) = 0;
    };
}
