#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event
{
    class MenuEvent : public ::jxx::lang::ClassBase<MenuEvent,
        ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<MenuEvent, JxxSuper>;
        explicit MenuEvent(const ::jxx::Ptr<::jxx::lang::Object>& source);
    };
}
