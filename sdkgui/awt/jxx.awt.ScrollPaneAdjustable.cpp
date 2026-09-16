#include "awt/jxx.awt.ScrollPaneAdjustable.h"

#include <algorithm>
#include <sstream>

#include "awt/event/jxx.awt.event.AdjustmentEvent.h"
#include "awt/event/jxx.awt.event.AdjustmentListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    ScrollPaneAdjustable::ScrollPaneAdjustable(::jxx::lang::jint orientation)
        : orientation_(orientation), value_(0), minimum_(0), maximum_(1),
          visibleAmount_(1), unitIncrement_(1), blockIncrement_(1),
          valueIsAdjusting_(false)
    {
        if (orientation != HORIZONTAL && orientation != VERTICAL)
            throw ::jxx::lang::IllegalArgumentException("orientation");
    }

    ::jxx::lang::jint ScrollPaneAdjustable::getOrientation() const { return orientation_; }
    void ScrollPaneAdjustable::setMinimum(::jxx::lang::jint minimum)
    {
        setSpan(minimum, maximum_, visibleAmount_);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getMinimum() const { return minimum_; }
    void ScrollPaneAdjustable::setMaximum(::jxx::lang::jint maximum)
    {
        setSpan(minimum_, maximum, visibleAmount_);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getMaximum() const { return maximum_; }
    void ScrollPaneAdjustable::setUnitIncrement(::jxx::lang::jint increment)
    {
        unitIncrement_ = std::max<::jxx::lang::jint>(1, increment);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getUnitIncrement() const { return unitIncrement_; }
    void ScrollPaneAdjustable::setBlockIncrement(::jxx::lang::jint increment)
    {
        blockIncrement_ = std::max<::jxx::lang::jint>(1, increment);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getBlockIncrement() const { return blockIncrement_; }
    void ScrollPaneAdjustable::setVisibleAmount(::jxx::lang::jint amount)
    {
        setSpan(minimum_, maximum_, amount);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getVisibleAmount() const { return visibleAmount_; }
    void ScrollPaneAdjustable::setValue(::jxx::lang::jint value)
    {
        setValueInternal(value, ::jxx::awt::event::AdjustmentEvent::TRACK,
            valueIsAdjusting_, true);
    }
    ::jxx::lang::jint ScrollPaneAdjustable::getValue() const { return value_; }
    ::jxx::lang::jbool ScrollPaneAdjustable::getValueIsAdjusting() const
    {
        return valueIsAdjusting_;
    }
    void ScrollPaneAdjustable::setValueIsAdjusting(::jxx::lang::jbool adjusting)
    {
        if (valueIsAdjusting_ == adjusting) return;
        valueIsAdjusting_ = adjusting;
        fireAdjustmentEvent(::jxx::awt::event::AdjustmentEvent::TRACK);
    }

    void ScrollPaneAdjustable::setSpan(::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum, ::jxx::lang::jint visibleAmount)
    {
        if (maximum <= minimum) maximum = minimum + 1;
        minimum_ = minimum;
        maximum_ = maximum;
        visibleAmount_ = std::max<::jxx::lang::jint>(1, visibleAmount);
        visibleAmount_ = std::min(visibleAmount_, maximum_ - minimum_);
        value_ = clampValue(value_);
        blockIncrement_ = std::max<::jxx::lang::jint>(1, visibleAmount_);
    }

    ::jxx::lang::jint ScrollPaneAdjustable::clampValue(
        ::jxx::lang::jint value) const
    {
        return std::max(minimum_, std::min(value, maximum_ - visibleAmount_));
    }

    void ScrollPaneAdjustable::setValueInternal(::jxx::lang::jint value,
        ::jxx::lang::jint adjustmentType, ::jxx::lang::jbool adjusting,
        ::jxx::lang::jbool fireEvent)
    {
        valueIsAdjusting_ = adjusting;
        const auto next = clampValue(value);
        if (next == value_) return;
        value_ = next;
        if (fireEvent) fireAdjustmentEvent(adjustmentType);
    }

    void ScrollPaneAdjustable::addAdjustmentListener(
        const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener)
    {
        if (listener != nullptr &&
            std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end())
            listeners_.push_back(listener);
    }

    void ScrollPaneAdjustable::removeAdjustmentListener(
        const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void ScrollPaneAdjustable::fireAdjustmentEvent(::jxx::lang::jint adjustmentType)
    {
        const auto event = ::jxx::NEW<::jxx::awt::event::AdjustmentEvent>(
            ::jxx::CAST<Adjustable>(thisPtr()),
            ::jxx::awt::event::AdjustmentEvent::ADJUSTMENT_VALUE_CHANGED,
            adjustmentType, value_, valueIsAdjusting_);
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->adjustmentValueChanged(event);
    }

    ::jxx::Ptr<::jxx::lang::String> ScrollPaneAdjustable::paramString() const
    {
        std::ostringstream out;
        out << (orientation_ == HORIZONTAL ? "horizontal" : "vertical")
            << ",value=" << value_ << ",min=" << minimum_
            << ",max=" << maximum_ << ",visibleAmount=" << visibleAmount_
            << ",unitIncrement=" << unitIncrement_
            << ",blockIncrement=" << blockIncrement_
            << ",isAdjusting=" << (valueIsAdjusting_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }

    ::jxx::Ptr<::jxx::lang::String> ScrollPaneAdjustable::toString() const
    {
        return ::jxx::NEW<::jxx::lang::String>(
            "jxx.awt.ScrollPaneAdjustable[" + paramString()->utf8() + "]");
    }
}
