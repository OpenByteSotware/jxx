#include "swing/jxx.swing.SpinnerNumberModel.h"

#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"

namespace jxx::swing
{
    SpinnerNumberModel::SpinnerNumberModel()
        : SpinnerNumberModel(0, 0, 100, 1)
    {
    }

    SpinnerNumberModel::SpinnerNumberModel(
        ::jxx::lang::jint value,
        ::jxx::lang::jint minimum,
        ::jxx::lang::jint maximum,
        ::jxx::lang::jint stepSize)
        : value_(::jxx::NEW<::jxx::lang::Integer>(value)),
          minimum_(::jxx::NEW<::jxx::lang::Integer>(minimum)),
          maximum_(::jxx::NEW<::jxx::lang::Integer>(maximum)),
          stepSize_(::jxx::NEW<::jxx::lang::Integer>(stepSize))
    {
        if (minimum > maximum || value < minimum || value > maximum)
            throw ::jxx::lang::IllegalArgumentException("range");
    }

    SpinnerNumberModel::SpinnerNumberModel(
        ::jxx::lang::jdouble value,
        ::jxx::lang::jdouble minimum,
        ::jxx::lang::jdouble maximum,
        ::jxx::lang::jdouble stepSize)
        : value_(::jxx::NEW<::jxx::lang::Double>(value)),
          minimum_(::jxx::NEW<::jxx::lang::Double>(minimum)),
          maximum_(::jxx::NEW<::jxx::lang::Double>(maximum)),
          stepSize_(::jxx::NEW<::jxx::lang::Double>(stepSize))
    {
        if (minimum > maximum || value < minimum || value > maximum)
            throw ::jxx::lang::IllegalArgumentException("range");
    }

    SpinnerNumberModel::SpinnerNumberModel(
        const ::jxx::Ptr<::jxx::lang::Number>& value,
        const ::jxx::Ptr<::jxx::lang::Object>& minimum,
        const ::jxx::Ptr<::jxx::lang::Object>& maximum,
        const ::jxx::Ptr<::jxx::lang::Number>& stepSize)
        : value_(value),
          minimum_(minimum),
          maximum_(maximum),
          stepSize_(stepSize)
    {
        if (value == nullptr || stepSize == nullptr)
            throw ::jxx::lang::IllegalArgumentException(
                "null value or stepSize");
        if ((minimum != nullptr && compareNumbers(value, minimum) < 0)
            || (maximum != nullptr && compareNumbers(value, maximum) > 0))
            throw ::jxx::lang::IllegalArgumentException("range");
    }

    ::jxx::Ptr<::jxx::lang::Object>
    SpinnerNumberModel::getValue() const
    {
        return value_;
    }

    void SpinnerNumberModel::setValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        const auto number = ::jxx::CAST<::jxx::lang::Number>(value);
        if (number == nullptr)
            throw ::jxx::lang::IllegalArgumentException("value");
        if (number != value_)
        {
            value_ = number;
            fireStateChanged();
        }
    }

    ::jxx::Ptr<::jxx::lang::Number> SpinnerNumberModel::adjacent(
        ::jxx::lang::jint direction) const
    {
        const auto integerValue = ::jxx::CAST<::jxx::lang::Integer>(value_);
        const auto integerStep = ::jxx::CAST<::jxx::lang::Integer>(stepSize_);
        if (integerValue != nullptr && integerStep != nullptr)
        {
            return ::jxx::NEW<::jxx::lang::Integer>(
                integerValue->intValue()
                + direction * integerStep->intValue());
        }

        const ::jxx::lang::jdouble result = value_->doubleValue()
            + static_cast<::jxx::lang::jdouble>(direction)
            * stepSize_->doubleValue();
        return ::jxx::NEW<::jxx::lang::Double>(result);
    }

    ::jxx::lang::jint SpinnerNumberModel::compareNumbers(
        const ::jxx::Ptr<::jxx::lang::Number>& left,
        const ::jxx::Ptr<::jxx::lang::Object>& right)
    {
        const auto rightNumber = ::jxx::CAST<::jxx::lang::Number>(right);
        if (left == nullptr || rightNumber == nullptr)
            throw ::jxx::lang::IllegalArgumentException("bound");
        const auto leftValue = left->doubleValue();
        const auto rightValue = rightNumber->doubleValue();
        return leftValue < rightValue ? -1 : (leftValue > rightValue ? 1 : 0);
    }

    ::jxx::Ptr<::jxx::lang::Object>
    SpinnerNumberModel::getNextValue() const
    {
        const auto next = adjacent(1);
        if (maximum_ != nullptr && compareNumbers(next, maximum_) > 0)
            return nullptr;
        return next;
    }

    ::jxx::Ptr<::jxx::lang::Object>
    SpinnerNumberModel::getPreviousValue() const
    {
        const auto previous = adjacent(-1);
        if (minimum_ != nullptr && compareNumbers(previous, minimum_) < 0)
            return nullptr;
        return previous;
    }

    ::jxx::Ptr<::jxx::lang::Number>
    SpinnerNumberModel::getNumber() const
    {
        return value_;
    }

    ::jxx::Ptr<::jxx::lang::Object>
    SpinnerNumberModel::getMinimum() const
    {
        return minimum_;
    }

    void SpinnerNumberModel::setMinimum(
        const ::jxx::Ptr<::jxx::lang::Object>& minimum)
    {
        if (minimum != nullptr
            && ::jxx::CAST<::jxx::lang::Number>(minimum) == nullptr)
            throw ::jxx::lang::IllegalArgumentException("minimum");
        if (minimum != minimum_)
        {
            minimum_ = minimum;
            fireStateChanged();
        }
    }

    ::jxx::Ptr<::jxx::lang::Object>
    SpinnerNumberModel::getMaximum() const
    {
        return maximum_;
    }

    void SpinnerNumberModel::setMaximum(
        const ::jxx::Ptr<::jxx::lang::Object>& maximum)
    {
        if (maximum != nullptr
            && ::jxx::CAST<::jxx::lang::Number>(maximum) == nullptr)
            throw ::jxx::lang::IllegalArgumentException("maximum");
        if (maximum != maximum_)
        {
            maximum_ = maximum;
            fireStateChanged();
        }
    }

    ::jxx::Ptr<::jxx::lang::Number>
    SpinnerNumberModel::getStepSize() const
    {
        return stepSize_;
    }

    void SpinnerNumberModel::setStepSize(
        const ::jxx::Ptr<::jxx::lang::Number>& stepSize)
    {
        if (stepSize == nullptr)
            throw ::jxx::lang::IllegalArgumentException("stepSize");
        if (stepSize != stepSize_)
        {
            stepSize_ = stepSize;
            fireStateChanged();
        }
    }
}
