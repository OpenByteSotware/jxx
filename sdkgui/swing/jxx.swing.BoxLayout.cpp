#include "swing/jxx.swing.BoxLayout.h"

#include <algorithm>
#include <limits>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::swing
{
    BoxLayout::BoxLayout(const ::jxx::Ptr<::jxx::awt::Container>& target,
        ::jxx::lang::jint axis)
        : Super(), target_(target), axis_(axis)
    {
        if (!target_) throw ::jxx::lang::NullPointerException("target");
        if (axis < X_AXIS || axis > PAGE_AXIS)
            throw ::jxx::lang::IllegalArgumentException("axis");
    }

    ::jxx::Ptr<::jxx::awt::Container> BoxLayout::getTarget() const
    { return target_; }

    ::jxx::lang::jint BoxLayout::getAxis() const { return axis_; }

    ::jxx::lang::jbool BoxLayout::horizontal() const
    {
        if (axis_ == X_AXIS) return true;
        if (axis_ == Y_AXIS) return false;
        if (axis_ == LINE_AXIS)
            return target_->getComponentOrientation()->isHorizontal();
        return !target_->getComponentOrientation()->isHorizontal();
    }

    void BoxLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::awt::Component>&) {}

    void BoxLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::lang::Object>&) {}

    void BoxLayout::removeLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>&) {}

    ::jxx::Ptr<::jxx::awt::Dimension> BoxLayout::preferredLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        ::jxx::lang::jint primary = 0;
        ::jxx::lang::jint secondary = 0;
        for (::jxx::lang::jint index = 0; index < target->getComponentCount(); ++index)
        {
            const auto component = target->getComponent(index);
            if (!component || !component->isVisible()) continue;
            const auto size = component->getSize();
            primary += horizontal() ? size->width : size->height;
            secondary = std::max(secondary,
                horizontal() ? size->height : size->width);
        }
        return horizontal()
            ? ::jxx::NEW<::jxx::awt::Dimension>(primary, secondary)
            : ::jxx::NEW<::jxx::awt::Dimension>(secondary, primary);
    }

    ::jxx::Ptr<::jxx::awt::Dimension> BoxLayout::minimumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    { return preferredLayoutSize(target); }

    ::jxx::Ptr<::jxx::awt::Dimension> BoxLayout::maximumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>&)
    {
        return ::jxx::NEW<::jxx::awt::Dimension>(
            std::numeric_limits<::jxx::lang::jint>::max(),
            std::numeric_limits<::jxx::lang::jint>::max());
    }

    ::jxx::lang::jfloat BoxLayout::getLayoutAlignmentX(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }

    ::jxx::lang::jfloat BoxLayout::getLayoutAlignmentY(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }

    void BoxLayout::invalidateLayout(
        const ::jxx::Ptr<::jxx::awt::Container>&) {}

    void BoxLayout::layoutContainer(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        ::jxx::lang::jint offset = 0;
        for (::jxx::lang::jint index = 0; index < target->getComponentCount(); ++index)
        {
            const auto component = target->getComponent(index);
            if (!component || !component->isVisible()) continue;
            const auto size = component->getSize();
            if (horizontal())
            {
                component->setBounds(offset, 0, size->width, target->getHeight());
                offset += size->width;
            }
            else
            {
                component->setBounds(0, offset, target->getWidth(), size->height);
                offset += size->height;
            }
        }
    }
}
