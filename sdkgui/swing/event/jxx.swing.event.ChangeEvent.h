#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event
{
    class ChangeEvent : public ::jxx::lang::ClassBase<ChangeEvent,
        ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<ChangeEvent, JxxSuper>;
        explicit ChangeEvent(const ::jxx::Ptr<::jxx::lang::Object>& source)
            : Super(source) {}
    };
}
