#include "swing/jxx.swing.Spring.h"

#include <algorithm>
#include <cmath>

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::swing
{
    Spring::Spring(::jxx::lang::jint minimum, ::jxx::lang::jint preferred,
        ::jxx::lang::jint maximum)
        : Super(), minimum_(minimum), preferred_(preferred), maximum_(maximum) {}

    ::jxx::lang::jint Spring::getMinimumValue() const { return minimum_; }
    ::jxx::lang::jint Spring::getPreferredValue() const { return preferred_; }
    ::jxx::lang::jint Spring::getMaximumValue() const { return maximum_; }
    ::jxx::lang::jint Spring::getValue() const
    { return value_ == UNSET ? preferred_ : value_; }
    void Spring::setValue(::jxx::lang::jint value) { value_ = value; }

    ::jxx::Ptr<Spring> Spring::constant(::jxx::lang::jint preferred)
    { return constant(preferred, preferred, preferred); }

    ::jxx::Ptr<Spring> Spring::constant(::jxx::lang::jint minimum,
        ::jxx::lang::jint preferred, ::jxx::lang::jint maximum)
    { return ::jxx::NEW<Spring>(minimum, preferred, maximum); }

    ::jxx::Ptr<Spring> Spring::minus(const ::jxx::Ptr<Spring>& spring)
    {
        if (!spring) throw ::jxx::lang::NullPointerException("spring");
        return constant(-spring->getMaximumValue(), -spring->getPreferredValue(),
            -spring->getMinimumValue());
    }

    ::jxx::Ptr<Spring> Spring::sum(const ::jxx::Ptr<Spring>& first,
        const ::jxx::Ptr<Spring>& second)
    {
        if (!first || !second) throw ::jxx::lang::NullPointerException("spring");
        return constant(first->getMinimumValue()+second->getMinimumValue(),
            first->getPreferredValue()+second->getPreferredValue(),
            first->getMaximumValue()+second->getMaximumValue());
    }

    ::jxx::Ptr<Spring> Spring::max(const ::jxx::Ptr<Spring>& first,
        const ::jxx::Ptr<Spring>& second)
    {
        if (!first || !second) throw ::jxx::lang::NullPointerException("spring");
        return constant(std::max(first->getMinimumValue(),second->getMinimumValue()),
            std::max(first->getPreferredValue(),second->getPreferredValue()),
            std::max(first->getMaximumValue(),second->getMaximumValue()));
    }

    ::jxx::Ptr<Spring> Spring::scale(const ::jxx::Ptr<Spring>& spring,
        ::jxx::lang::jfloat factor)
    {
        if (!spring) throw ::jxx::lang::NullPointerException("spring");
        return constant(static_cast<::jxx::lang::jint>(std::lround(
            spring->getMinimumValue()*factor)),
            static_cast<::jxx::lang::jint>(std::lround(
                spring->getPreferredValue()*factor)),
            static_cast<::jxx::lang::jint>(std::lround(
                spring->getMaximumValue()*factor)));
    }
}
