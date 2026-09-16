#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Color;
    class Font;
}

namespace jxx::gui::internal
{
    class NativeComponent : public ::jxx::lang::Object
    {
    public:
        ~NativeComponent() override = default;

        virtual void destroy() = 0;
        virtual void setVisible(::jxx::lang::jbool visible) = 0;
        virtual void setEnabled(::jxx::lang::jbool enabled) = 0;
        virtual void setBounds(
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height) = 0;

        virtual void setText(
            const ::jxx::Ptr<::jxx::lang::String>& text) = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getText() const = 0;

        virtual void setEditable(::jxx::lang::jbool editable) = 0;
        virtual void setSelection(
            ::jxx::lang::jint start,
            ::jxx::lang::jint end) = 0;
        virtual void setCaretPosition(::jxx::lang::jint position) = 0;

        virtual void setForeground(
            const ::jxx::Ptr<::jxx::awt::Color>& color) = 0;
        virtual void setBackground(
            const ::jxx::Ptr<::jxx::awt::Color>& color) = 0;
        virtual void setFont(
            const ::jxx::Ptr<::jxx::awt::Font>& font) = 0;
    };
}
