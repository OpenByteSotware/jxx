#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event
{
    class CaretEvent : public ::jxx::lang::ClassBase<CaretEvent,
        ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<CaretEvent, JxxSuper>;
        ~CaretEvent() override = default;
        virtual ::jxx::lang::jint getDot() const = 0;
        virtual ::jxx::lang::jint getMark() const = 0;
    protected:
        explicit CaretEvent(const ::jxx::Ptr<::jxx::lang::Object>& source)
            : Super(source) {}
    };
}
