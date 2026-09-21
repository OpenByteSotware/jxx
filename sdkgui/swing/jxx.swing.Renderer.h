#pragma once
#include "awt/jxx.awt.Component.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class Renderer : public ::jxx::lang::InterfaceBase<Renderer>
    {
    public:
        ~Renderer() override = default;

        virtual void setValue(
            const ::jxx::Ptr<::jxx::lang::Object>& value,
            ::jxx::lang::jbool isSelected) = 0;
        virtual ::jxx::Ptr<::jxx::awt::Component> getComponent() const = 0;
    };
}
