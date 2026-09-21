#pragma once

#include "awt/jxx.awt.Container.h"

namespace jxx::awt
{
    class Graphics;
}

namespace jxx::swing
{
    class CellRendererPane : public ::jxx::lang::ClassBase<
        CellRendererPane, ::jxx::awt::Container>
    {
    public:
        using JxxSuper = ::jxx::awt::Container;
        using Super = ::jxx::lang::ClassBase<CellRendererPane, JxxSuper>;

        CellRendererPane();
        ~CellRendererPane() override = default;

        void invalidate() override;

        void paintComponent(
            const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
            const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::awt::Container>& parent,
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height,
            ::jxx::lang::jbool shouldValidate);

        void paintComponent(
            const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
            const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::awt::Container>& parent,
            const ::jxx::Ptr<::jxx::awt::Rectangle>& rectangle);
    };
}
