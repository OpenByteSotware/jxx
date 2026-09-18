#pragma once

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    class JTextComponent : public ::jxx::lang::ClassBase<JTextComponent, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JTextComponent, JxxSuper>;
        ~JTextComponent() override = 0;
        ::jxx::Ptr<::jxx::lang::String> getText() const;
        virtual void setText(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::lang::jbool isEditable() const;
        void setEditable(::jxx::lang::jbool editable);
        ::jxx::lang::jint getCaretPosition() const;
        void setCaretPosition(::jxx::lang::jint position);
        void select(::jxx::lang::jint start, ::jxx::lang::jint end);
        void selectAll();
        ::jxx::Ptr<::jxx::lang::String> getSelectedText() const;
    protected:
        JTextComponent();
        explicit JTextComponent(const ::jxx::Ptr<::jxx::lang::String>& text);
    private:
        ::jxx::Ptr<::jxx::lang::String> text_;
        ::jxx::lang::jbool editable_ = true;
        ::jxx::lang::jint selectionStart_ = 0;
        ::jxx::lang::jint selectionEnd_ = 0;
        ::jxx::lang::jint caretPosition_ = 0;
    };
}
