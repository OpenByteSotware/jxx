#pragma once

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    class JPanel : public ::jxx::lang::ClassBase<JPanel, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JPanel, JxxSuper>;
        JPanel();
        explicit JPanel(::jxx::lang::jbool doubleBuffered);
        explicit JPanel(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout);
        JPanel(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout,
            ::jxx::lang::jbool doubleBuffered);
        ~JPanel() override = default;
    };
}
