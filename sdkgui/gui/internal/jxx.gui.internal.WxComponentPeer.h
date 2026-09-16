#pragma once

#include <functional>

#include "gui/internal/jxx.gui.internal.NativeComponent.h"

class wxWindow;

namespace jxx::gui::internal
{
    class WxComponentPeer final : public NativeComponent
    {
    public:
        using ActionCallback = std::function<void()>;

        explicit WxComponentPeer(wxWindow* window);
        ~WxComponentPeer() override;

        void destroy() override;
        void setVisible(::jxx::lang::jbool visible) override;
        void setEnabled(::jxx::lang::jbool enabled) override;
        void setBounds(
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height) override;

        void setText(
            const ::jxx::Ptr<::jxx::lang::String>& text) override;
        ::jxx::Ptr<::jxx::lang::String> getText() const override;

        void setEditable(::jxx::lang::jbool editable) override;
        void setSelection(
            ::jxx::lang::jint start,
            ::jxx::lang::jint end) override;
        void setCaretPosition(::jxx::lang::jint position) override;

        void setForeground(
            const ::jxx::Ptr<::jxx::awt::Color>& color) override;
        void setBackground(
            const ::jxx::Ptr<::jxx::awt::Color>& color) override;
        void setFont(
            const ::jxx::Ptr<::jxx::awt::Font>& font) override;

        void setActionCallback(ActionCallback callback);
        wxWindow* nativeWindow() const;

    private:
        wxWindow* window_;
        ActionCallback actionCallback_;
    };
}
