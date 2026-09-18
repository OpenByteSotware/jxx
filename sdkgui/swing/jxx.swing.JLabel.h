#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::swing
{
    class JLabel : public ::jxx::lang::ClassBase<JLabel, JComponent, SwingConstants>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JLabel, JxxSuper, SwingConstants>;
        JLabel();
        explicit JLabel(const ::jxx::Ptr<::jxx::lang::String>& text);
        JLabel(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jint alignment);
        ::jxx::Ptr<::jxx::lang::String> getText() const;
        void setText(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::lang::jint getHorizontalAlignment() const;
        void setHorizontalAlignment(::jxx::lang::jint alignment);
    private:
        ::jxx::Ptr<::jxx::lang::String> text_;
        ::jxx::lang::jint alignment_;
    };
}
