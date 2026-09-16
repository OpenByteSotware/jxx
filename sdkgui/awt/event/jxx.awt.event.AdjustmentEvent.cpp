#include "awt/event/jxx.awt.event.AdjustmentEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    AdjustmentEvent::AdjustmentEvent(
        const ::jxx::Ptr<::jxx::awt::Adjustable>& source,
        ::jxx::lang::jint id,
        ::jxx::lang::jint type,
        ::jxx::lang::jint value)
        : AdjustmentEvent(source, id, type, value, false)
    {
    }

    AdjustmentEvent::AdjustmentEvent(
        const ::jxx::Ptr<::jxx::awt::Adjustable>& source,
        ::jxx::lang::jint id,
        ::jxx::lang::jint type,
        ::jxx::lang::jint value,
        ::jxx::lang::jbool valueIsAdjusting)
        : Super(::jxx::CAST<::jxx::lang::Object>(source), id),
          adjustmentType_(type),
          value_(value),
          valueIsAdjusting_(valueIsAdjusting)
    {
    }

    ::jxx::Ptr<::jxx::awt::Adjustable> AdjustmentEvent::getAdjustable() const
    {
        return ::jxx::CAST<::jxx::awt::Adjustable>(source);
    }

    ::jxx::lang::jint AdjustmentEvent::getAdjustmentType() const
    {
        return adjustmentType_;
    }

    ::jxx::lang::jint AdjustmentEvent::getValue() const
    {
        return value_;
    }

    ::jxx::lang::jbool AdjustmentEvent::getValueIsAdjusting() const
    {
        return valueIsAdjusting_;
    }

    ::jxx::Ptr<::jxx::lang::String> AdjustmentEvent::paramString() const
    {
        const char* typeName = "unknown type";
        switch (adjustmentType_)
        {
            case UNIT_INCREMENT: typeName = "UNIT_INCREMENT"; break;
            case UNIT_DECREMENT: typeName = "UNIT_DECREMENT"; break;
            case BLOCK_DECREMENT: typeName = "BLOCK_DECREMENT"; break;
            case BLOCK_INCREMENT: typeName = "BLOCK_INCREMENT"; break;
            case TRACK: typeName = "TRACK"; break;
            default: break;
        }

        std::ostringstream out;
        out << "ADJUSTMENT_VALUE_CHANGED,adjType=" << typeName
            << ",value=" << value_
            << ",isAdjusting=" << (valueIsAdjusting_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
