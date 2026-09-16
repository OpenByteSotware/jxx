#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::gui::internal
{
    class NativeWindow : public ::jxx::lang::Object
    {
    public:
        ~NativeWindow() override = default;

        virtual void show() = 0;
        virtual void hide() = 0;

        virtual void setTitle(
            const ::jxx::Ptr<::jxx::lang::String>& title) = 0;

        virtual ::jxx::Ptr<::jxx::lang::String> getTitle() const = 0;
    };
}
