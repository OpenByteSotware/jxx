#pragma once

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    class Box : public ::jxx::lang::ClassBase<Box, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<Box, JxxSuper>;

        explicit Box(::jxx::lang::jint axis);
        static ::jxx::Ptr<Box> createHorizontalBox();
        static ::jxx::Ptr<Box> createVerticalBox();
        static ::jxx::Ptr<::jxx::awt::Component> createRigidArea(
            const ::jxx::Ptr<::jxx::awt::Dimension>& dimension);
        static ::jxx::Ptr<::jxx::awt::Component> createHorizontalStrut(
            ::jxx::lang::jint width);
        static ::jxx::Ptr<::jxx::awt::Component> createVerticalStrut(
            ::jxx::lang::jint height);
        static ::jxx::Ptr<::jxx::awt::Component> createGlue();
        static ::jxx::Ptr<::jxx::awt::Component> createHorizontalGlue();
        static ::jxx::Ptr<::jxx::awt::Component> createVerticalGlue();
    };
}
