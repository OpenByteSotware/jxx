#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::beans
{
    class PropertyChangeEvent;

    class PropertyChangeListener : public ::jxx::lang::InterfaceBase<
        PropertyChangeListener>
    {
    public:
        ~PropertyChangeListener() override = default;
        virtual void propertyChange(
            const ::jxx::Ptr<PropertyChangeEvent>& event) = 0;
    };
}
