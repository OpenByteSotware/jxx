#include "awt/jxx.awt.Scrollbar.h"

#include <algorithm>
#include <limits>
#include <sstream>

#include "awt/event/jxx.awt.event.AdjustmentEvent.h"
#include "awt/event/jxx.awt.event.AdjustmentListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    Scrollbar::Scrollbar() : Scrollbar(VERTICAL, 0, 10, 0, 100) {}

    Scrollbar::Scrollbar(::jxx::lang::jint orientation)
        : Scrollbar(orientation, 0, 10, 0, 100) {}

    Scrollbar::Scrollbar(::jxx::lang::jint orientation,
        ::jxx::lang::jint value,
        ::jxx::lang::jint visibleAmount,
        ::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum)
        : orientation_(orientation), value_(0), visibleAmount_(1),
          minimum_(0), maximum_(100), unitIncrement_(1),
          blockIncrement_(10), valueIsAdjusting_(false)
    {
        if (orientation != HORIZONTAL && orientation != VERTICAL)
            throw ::jxx::lang::IllegalArgumentException("orientation");
        normalizeValues(value, visibleAmount, minimum, maximum);
    }

    ::jxx::lang::jint Scrollbar::getOrientation() const { return orientation_; }

    void Scrollbar::normalizeValues(::jxx::lang::jint value,
        ::jxx::lang::jint visibleAmount,
        ::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum)
    {
        if (maximum <= minimum)
            maximum = minimum == std::numeric_limits<::jxx::lang::jint>::max()
                ? minimum : minimum + 1;
        visibleAmount = std::max<::jxx::lang::jint>(1, visibleAmount);
        visibleAmount = std::min(visibleAmount, maximum - minimum);
        value = std::max(minimum, value);
        value = std::min(value, maximum - visibleAmount);
        minimum_ = minimum;
        maximum_ = maximum;
        visibleAmount_ = visibleAmount;
        value_ = value;
    }

    void Scrollbar::setValues(::jxx::lang::jint value,
        ::jxx::lang::jint visibleAmount,
        ::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum)
    {
        normalizeValues(value, visibleAmount, minimum, maximum);
        invalidate();
    }

    ::jxx::lang::jint Scrollbar::getValue() const { return value_; }
    void Scrollbar::setValue(::jxx::lang::jint value)
    {
        normalizeValues(value, visibleAmount_, minimum_, maximum_);
    }
    ::jxx::lang::jint Scrollbar::getMinimum() const { return minimum_; }
    void Scrollbar::setMinimum(::jxx::lang::jint minimum)
    {
        normalizeValues(value_, visibleAmount_, minimum, maximum_);
    }
    ::jxx::lang::jint Scrollbar::getMaximum() const { return maximum_; }
    void Scrollbar::setMaximum(::jxx::lang::jint maximum)
    {
        normalizeValues(value_, visibleAmount_, minimum_, maximum);
    }
    ::jxx::lang::jint Scrollbar::getVisibleAmount() const { return visibleAmount_; }
    void Scrollbar::setVisibleAmount(::jxx::lang::jint amount)
    {
        normalizeValues(value_, amount, minimum_, maximum_);
    }
    ::jxx::lang::jint Scrollbar::getUnitIncrement() const { return unitIncrement_; }
    void Scrollbar::setUnitIncrement(::jxx::lang::jint increment)
    {
        unitIncrement_ = std::max<::jxx::lang::jint>(1, increment);
    }
    ::jxx::lang::jint Scrollbar::getLineIncrement() const { return getUnitIncrement(); }
    void Scrollbar::setLineIncrement(::jxx::lang::jint increment) { setUnitIncrement(increment); }
    ::jxx::lang::jint Scrollbar::getBlockIncrement() const { return blockIncrement_; }
    void Scrollbar::setBlockIncrement(::jxx::lang::jint increment)
    {
        blockIncrement_ = std::max<::jxx::lang::jint>(1, increment);
    }
    ::jxx::lang::jint Scrollbar::getPageIncrement() const { return getBlockIncrement(); }
    void Scrollbar::setPageIncrement(::jxx::lang::jint increment) { setBlockIncrement(increment); }
    ::jxx::lang::jbool Scrollbar::getValueIsAdjusting() const { return valueIsAdjusting_; }
    void Scrollbar::setValueIsAdjusting(::jxx::lang::jbool adjusting)
    {
        valueIsAdjusting_ = adjusting;
    }

    void Scrollbar::addAdjustmentListener(
        const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener)
    {
        if (listener != nullptr &&
            std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end())
            listeners_.push_back(listener);
    }

    void Scrollbar::removeAdjustmentListener(
        const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void Scrollbar::setValueFromNative(::jxx::lang::jint value,
        ::jxx::lang::jint adjustmentType,
        ::jxx::lang::jbool adjusting)
    {
        const auto previous = value_;
        normalizeValues(value, visibleAmount_, minimum_, maximum_);
        valueIsAdjusting_ = adjusting;
        if (previous == value_) return;
        processAdjustmentEvent(::jxx::NEW<::jxx::awt::event::AdjustmentEvent>(
            ::jxx::CAST<Adjustable>(thisPtr()),
            ::jxx::awt::event::AdjustmentEvent::ADJUSTMENT_VALUE_CHANGED,
            adjustmentType, value_, valueIsAdjusting_));
    }

    ::jxx::Ptr<::jxx::lang::String> Scrollbar::paramString() const
    {
        std::ostringstream out;
        out << "val=" << value_
            << ",vis=" << visibleAmount_
            << ",min=" << minimum_
            << ",max=" << maximum_
            << ",orient=" << (orientation_ == HORIZONTAL
                ? "horizontal" : "vertical")
            << ",isAdjusting=" << (valueIsAdjusting_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }

    void Scrollbar::processAdjustmentEvent(
        const ::jxx::Ptr<::jxx::awt::event::AdjustmentEvent>& event)
    {
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->adjustmentValueChanged(event);
    }
}
