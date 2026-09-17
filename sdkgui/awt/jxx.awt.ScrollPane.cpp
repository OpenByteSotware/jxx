#include "awt/jxx.awt.ScrollPane.h"

#include <algorithm>
#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    ScrollPane::ScrollPane() : ScrollPane(SCROLLBARS_AS_NEEDED) {}

    ScrollPane::ScrollPane(::jxx::lang::jint policy)
        : scrollbarDisplayPolicy_(policy),
          horizontal_(::jxx::Ptr<ScrollPaneAdjustable>(
              new ScrollPaneAdjustable(Adjustable::HORIZONTAL))),
          vertical_(::jxx::Ptr<ScrollPaneAdjustable>(
              new ScrollPaneAdjustable(Adjustable::VERTICAL))),
          wheelScrollingEnabled_(true)
    {
        if (policy != SCROLLBARS_AS_NEEDED && policy != SCROLLBARS_ALWAYS &&
            policy != SCROLLBARS_NEVER)
            throw ::jxx::lang::IllegalArgumentException("scrollbarDisplayPolicy");
        setSize(100, 100);
    }

    ::jxx::lang::jint ScrollPane::getScrollbarDisplayPolicy() const
    {
        return scrollbarDisplayPolicy_;
    }

    ::jxx::Ptr<Adjustable> ScrollPane::getHAdjustable() const
    {
        return ::jxx::CAST<Adjustable>(horizontal_);
    }

    ::jxx::Ptr<Adjustable> ScrollPane::getVAdjustable() const
    {
        return ::jxx::CAST<Adjustable>(vertical_);
    }

    ::jxx::Ptr<Point> ScrollPane::getScrollPosition() const
    {
        if (getComponentCount() == 0)
            throw ::jxx::lang::NullPointerException("child");
        return ::jxx::NEW<Point>(horizontal_->getValue(), vertical_->getValue());
    }

    void ScrollPane::setScrollPosition(const ::jxx::Ptr<Point>& position)
    {
        if (position == nullptr)
            throw ::jxx::lang::NullPointerException("position");
        setScrollPosition(position->x, position->y);
    }

    void ScrollPane::setScrollPosition(::jxx::lang::jint x,
        ::jxx::lang::jint y)
    {
        if (getComponentCount() == 0)
            throw ::jxx::lang::NullPointerException("child");
        horizontal_->setValue(x);
        vertical_->setValue(y);
        doLayout();
    }

    ::jxx::Ptr<Dimension> ScrollPane::getViewportSize() const
    {
        return ::jxx::NEW<Dimension>(getWidth(), getHeight());
    }

    ::jxx::lang::jint ScrollPane::getHScrollbarHeight() const { return 0; }
    ::jxx::lang::jint ScrollPane::getVScrollbarWidth() const { return 0; }
    ::jxx::lang::jbool ScrollPane::isWheelScrollingEnabled() const
    {
        return wheelScrollingEnabled_;
    }
    void ScrollPane::setWheelScrollingEnabled(::jxx::lang::jbool enabled)
    {
        wheelScrollingEnabled_ = enabled;
    }

    ::jxx::Ptr<Component> ScrollPane::add(
        const ::jxx::Ptr<Component>& component)
    {
        return add(component, 0);
    }

    ::jxx::Ptr<Component> ScrollPane::add(
        const ::jxx::Ptr<Component>& component, ::jxx::lang::jint index)
    {
        if (component == nullptr)
            throw ::jxx::lang::NullPointerException("component");
        if (index != 0)
            throw ::jxx::lang::IllegalArgumentException("index");
        if (getComponentCount() != 0) remove(0);
        const auto result = Container::add(component, 0);
        updateAdjustables();
        return result;
    }

    ::jxx::Ptr<Component> ScrollPane::add(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<Component>& component)
    {
        return add(component);
    }

    void ScrollPane::updateAdjustables()
    {
        const auto child = getComponentCount() == 0 ? nullptr : getComponent(0);
        const auto childWidth = child == nullptr ? getWidth() : child->getWidth();
        const auto childHeight = child == nullptr ? getHeight() : child->getHeight();
        horizontal_->setSpan(0, std::max(getWidth(), childWidth), getWidth());
        vertical_->setSpan(0, std::max(getHeight(), childHeight), getHeight());
    }

    void ScrollPane::doLayout()
    {
        updateAdjustables();
        if (getComponentCount() == 0) return;
        const auto child = getComponent(0);
        child->setLocation(-horizontal_->getValue(), -vertical_->getValue());
    }

    ::jxx::Ptr<::jxx::lang::String> ScrollPane::paramString() const
    {
        const char* policy = scrollbarDisplayPolicy_ == SCROLLBARS_ALWAYS
            ? "always" : scrollbarDisplayPolicy_ == SCROLLBARS_NEVER
                ? "never" : "as-needed";
        std::ostringstream out;
        out << "scrollbarDisplayPolicy=" << policy
            << ",wheelScrollingEnabled="
            << (wheelScrollingEnabled_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
