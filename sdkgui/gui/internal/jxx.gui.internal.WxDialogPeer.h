#pragma once

#include "gui/internal/jxx.gui.internal.NativeWindow.h"

class wxDialog;
class wxWindow;

namespace jxx::awt
{
    class Container;
}

namespace jxx::gui::internal
{
    class WxDialogPeer final : public NativeWindow
    {
    public:
        WxDialogPeer(const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool resizable,
            ::jxx::lang::jbool undecorated);
        ~WxDialogPeer() override;

        void show() override;
        void showModal();
        void hide() override;
        void destroy() override;
        void toFront() override;
        void toBack() override;
        void setBounds(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void setTitle(const ::jxx::Ptr<::jxx::lang::String>& title) override;
        ::jxx::Ptr<::jxx::lang::String> getTitle() const override;
        ::jxx::lang::jbool isShown() const override;
        void setEventCallback(EventCallback callback) override;
        void setMenuBar(const ::jxx::Ptr<::jxx::awt::MenuBar>& menuBar) override;
        void installComponents(const ::jxx::Ptr<::jxx::awt::Container>& container);

    private:
        void send(::jxx::lang::jint id);
        void buildChildren(wxWindow* parent,
            const ::jxx::Ptr<::jxx::awt::Container>& container);

        wxDialog* dialog_;
        ::jxx::Ptr<::jxx::lang::String> title_;
        EventCallback callback_;
    };
}
