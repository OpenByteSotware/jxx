#include "swing/jxx.swing.CellRendererPane.h"

namespace jxx::swing
{
    CellRendererPane::CellRendererPane() : Super()
    {
        setVisible(false);
    }

    void CellRendererPane::invalidate()
    {
    }

    void CellRendererPane::paintComponent(
        const ::jxx::Ptr<::jxx::awt::Graphics>&,
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        const ::jxx::Ptr<::jxx::awt::Container>& parent,
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height,
        ::jxx::lang::jbool shouldValidate)
    {
        if (component == nullptr) return;

        const auto existingParent = component->getParent();
        if (existingParent == nullptr)
            add(component);

        component->setBounds(x, y, width, height);
        if (shouldValidate) component->validate();

        if (parent != nullptr && component->getParent() == nullptr)
            parent->add(component);

        if (existingParent == nullptr && component->getParent().get() == this)
            remove(component);
    }

    void CellRendererPane::paintComponent(
        const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        const ::jxx::Ptr<::jxx::awt::Container>& parent,
        const ::jxx::Ptr<::jxx::awt::Rectangle>& rectangle)
    {
        if (rectangle == nullptr) return;
        paintComponent(graphics, component, parent,
            rectangle->x, rectangle->y,
            rectangle->width, rectangle->height, false);
    }
}
