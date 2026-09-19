#include "swing/jxx.swing.SpringLayout.h"

#include <algorithm>
#include <limits>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing
{
    SpringLayout::Constraints::Constraints()
        : Constraints(Spring::constant(0), Spring::constant(0)) {}
    SpringLayout::Constraints::Constraints(const ::jxx::Ptr<Spring>& x,
        const ::jxx::Ptr<Spring>& y) : Super(), x_(x), y_(y) {}
    void SpringLayout::Constraints::setX(const ::jxx::Ptr<Spring>& value)
    { x_ = value; }
    ::jxx::Ptr<Spring> SpringLayout::Constraints::getX() const { return x_; }
    void SpringLayout::Constraints::setY(const ::jxx::Ptr<Spring>& value)
    { y_ = value; }
    ::jxx::Ptr<Spring> SpringLayout::Constraints::getY() const { return y_; }
    void SpringLayout::Constraints::setWidth(const ::jxx::Ptr<Spring>& value)
    { width_ = value; }
    ::jxx::Ptr<Spring> SpringLayout::Constraints::getWidth() const { return width_; }
    void SpringLayout::Constraints::setHeight(const ::jxx::Ptr<Spring>& value)
    { height_ = value; }
    ::jxx::Ptr<Spring> SpringLayout::Constraints::getHeight() const { return height_; }

    ::jxx::Ptr<SpringLayout::Constraints> SpringLayout::getConstraints(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    {
        auto& value = constraints_[component.get()];
        if (!value)
        {
            value = ::jxx::NEW<Constraints>();
            value->setWidth(Spring::constant(component ? component->getWidth() : 0));
            value->setHeight(Spring::constant(component ? component->getHeight() : 0));
        }
        return value;
    }

    ::jxx::Ptr<Spring> SpringLayout::getConstraint(
        const ::jxx::Ptr<::jxx::lang::String>& edge,
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    {
        const auto value = edge ? edge->utf8() : std::string();
        const auto constraints = getConstraints(component);
        if (value == WEST) return constraints->getX();
        if (value == NORTH) return constraints->getY();
        if (value == WIDTH) return constraints->getWidth();
        if (value == HEIGHT) return constraints->getHeight();
        if (value == EAST) return Spring::sum(constraints->getX(),
            constraints->getWidth());
        if (value == SOUTH) return Spring::sum(constraints->getY(),
            constraints->getHeight());
        return Spring::constant(0);
    }

    void SpringLayout::putConstraint(
        const ::jxx::Ptr<::jxx::lang::String>& edge1,
        const ::jxx::Ptr<::jxx::awt::Component>& component1,
        ::jxx::lang::jint padding,
        const ::jxx::Ptr<::jxx::lang::String>& edge2,
        const ::jxx::Ptr<::jxx::awt::Component>& component2)
    {
        const auto edge = edge1 ? edge1->utf8() : std::string();
        const auto spring = Spring::sum(getConstraint(edge2, component2),
            Spring::constant(padding));
        const auto constraints = getConstraints(component1);
        if (edge == WEST) constraints->setX(spring);
        else if (edge == NORTH) constraints->setY(spring);
        else if (edge == WIDTH) constraints->setWidth(spring);
        else if (edge == HEIGHT) constraints->setHeight(spring);
    }

    void SpringLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    { getConstraints(component); }

    void SpringLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        const ::jxx::Ptr<::jxx::lang::Object>& constraints)
    {
        const auto typed = ::jxx::CAST<Constraints>(constraints);
        constraints_[component.get()] = typed ? typed : ::jxx::NEW<Constraints>();
    }

    void SpringLayout::removeLayoutComponent(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    { constraints_.erase(component.get()); }

    ::jxx::Ptr<::jxx::awt::Dimension> SpringLayout::preferredLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        ::jxx::lang::jint width = 0;
        ::jxx::lang::jint height = 0;
        for (::jxx::lang::jint index=0; index<target->getComponentCount(); ++index)
        {
            const auto component = target->getComponent(index);
            const auto constraints = getConstraints(component);
            width = std::max(width, constraints->getX()->getValue()
                + constraints->getWidth()->getValue());
            height = std::max(height, constraints->getY()->getValue()
                + constraints->getHeight()->getValue());
        }
        return ::jxx::NEW<::jxx::awt::Dimension>(width, height);
    }

    ::jxx::Ptr<::jxx::awt::Dimension> SpringLayout::minimumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    { return preferredLayoutSize(target); }

    ::jxx::Ptr<::jxx::awt::Dimension> SpringLayout::maximumLayoutSize(
        const ::jxx::Ptr<::jxx::awt::Container>&)
    {
        return ::jxx::NEW<::jxx::awt::Dimension>(
            std::numeric_limits<::jxx::lang::jint>::max(),
            std::numeric_limits<::jxx::lang::jint>::max());
    }

    ::jxx::lang::jfloat SpringLayout::getLayoutAlignmentX(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }
    ::jxx::lang::jfloat SpringLayout::getLayoutAlignmentY(
        const ::jxx::Ptr<::jxx::awt::Container>&) { return 0.5F; }
    void SpringLayout::invalidateLayout(
        const ::jxx::Ptr<::jxx::awt::Container>&) {}

    void SpringLayout::layoutContainer(
        const ::jxx::Ptr<::jxx::awt::Container>& target)
    {
        for (::jxx::lang::jint index=0; index<target->getComponentCount(); ++index)
        {
            const auto component = target->getComponent(index);
            const auto constraints = getConstraints(component);
            component->setBounds(constraints->getX()->getValue(),
                constraints->getY()->getValue(),
                constraints->getWidth()->getValue(),
                constraints->getHeight()->getValue());
        }
    }
}
