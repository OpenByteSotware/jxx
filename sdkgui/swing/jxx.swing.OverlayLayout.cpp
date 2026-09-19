#include "swing/jxx.swing.OverlayLayout.h"

#include <algorithm>
#include <limits>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::swing
{
    OverlayLayout::OverlayLayout(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
        : Super(), target_(target)
    {
        if (!target_) throw ::jxx::lang::NullPointerException("target");
    }

    ::jxx::Ptr<::jxx::awt::Container> OverlayLayout::getTarget() const
    { return target_; }

    void OverlayLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::awt::Component>&) {}

    void OverlayLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::lang::Object>&) {}

    void OverlayLayout::removeLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>&) {}

    ::jxx::Ptr<::jxx::awt::Dimension> OverlayLayout::preferredLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        ::jxx::lang::jint width = 0;
        ::jxx::lang::jint height = 0;
        for (::jxx::lang::jint index = 0; index < target->getComponentCount(); ++index)
        {
            const auto size = target->getComponent(index)->getSize();
            width = std::max(width, size->width);
            height = std::max(height, size->height);
        }
        return ::jxx::NEW<::jxx::awt::Dimension>(width, height);
    }

    ::jxx::Ptr<::jxx::awt::Dimension> OverlayLayout::minimumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    { return preferredLayoutSize(target); }

    ::jxx::Ptr<::jxx::awt::Dimension> OverlayLayout::maximumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>&)
    {
        return ::jxx::NEW<::jxx::awt::Dimension>(
            std::numeric_limits<::jxx::lang::jint>::max(),
            std::numeric_limits<::jxx::lang::jint>::max());
    }

    ::jxx::lang::jfloat OverlayLayout::getLayoutAlignmentX(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }

    ::jxx::lang::jfloat OverlayLayout::getLayoutAlignmentY(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }

    void OverlayLayout::invalidateLayout(
        const ::jxx::Ptr<::jxx::awt::Container>&) {}

    void OverlayLayout::layoutContainer(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        for (::jxx::lang::jint index = 0; index < target->getComponentCount(); ++index)
        {
            const auto component = target->getComponent(index);
            if (component) component->setBounds(0, 0,
                target->getWidth(), target->getHeight());
        }
    }
}
