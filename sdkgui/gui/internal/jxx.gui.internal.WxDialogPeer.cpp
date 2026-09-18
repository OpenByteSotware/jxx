#include "gui/internal/jxx.gui.internal.WxDialogPeer.h"
#include <utility>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Label.h"
#include "awt/jxx.awt.TextField.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
#include "gui/internal/jxx.gui.internal.WxComponentPeer.h"
#include "swing/jxx.swing.AbstractButton.h"
#include "swing/jxx.swing.JLabel.h"
namespace jxx::gui::internal
{
    namespace
    {
        wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& value)
        {
            return value != nullptr
                ? wxString::FromUTF8(value->utf8().c_str()) : wxString();
        }
    }
    WxDialogPeer::WxDialogPeer(const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool resizable, ::jxx::lang::jbool undecorated)
        : dialog_(nullptr), title_(title != nullptr ? title
            : ::jxx::NEW<::jxx::lang::String>(""))
    {
        if (!GuiRuntime::ensureInitialized()) return;
        long style = undecorated ? wxBORDER_NONE : wxDEFAULT_DIALOG_STYLE;
        if (resizable) style |= wxRESIZE_BORDER;
        dialog_ = new wxDialog(nullptr, wxID_ANY, nativeText(title_),
            wxDefaultPosition, wxDefaultSize, style);
        dialog_->Bind(wxEVT_SHOW, [this](wxShowEvent& event)
        {
            send(event.IsShown() ? ::jxx::awt::event::WindowEvent::WINDOW_OPENED
                : ::jxx::awt::event::WindowEvent::WINDOW_CLOSED);
            event.Skip();
        });
        dialog_->Bind(wxEVT_ACTIVATE, [this](wxActivateEvent& event)
        {
            send(event.GetActive() ? ::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED
                : ::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED);
            event.Skip();
        });
        dialog_->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event)
        {
            send(::jxx::awt::event::WindowEvent::WINDOW_CLOSING);
            if (dialog_ != nullptr && dialog_->IsModal()) dialog_->EndModal(wxID_CANCEL);
            else event.Skip();
        });
        dialog_->Bind(wxEVT_DESTROY, [this](wxWindowDestroyEvent& event)
        {
            send(::jxx::awt::event::WindowEvent::WINDOW_CLOSED);
            dialog_ = nullptr;
            event.Skip();
        });
    }
    WxDialogPeer::~WxDialogPeer() { destroy(); }
    void WxDialogPeer::send(::jxx::lang::jint id) { if (callback_) callback_(id); }
    void WxDialogPeer::setEventCallback(EventCallback callback) { callback_ = std::move(callback); }
    void WxDialogPeer::show() { if (dialog_ != nullptr) dialog_->Show(true); }
    void WxDialogPeer::showModal() { if (dialog_ != nullptr && !dialog_->IsModal()) dialog_->ShowModal(); }
    void WxDialogPeer::hide() { if (dialog_ == nullptr) return; if (dialog_->IsModal()) dialog_->EndModal(wxID_OK); else dialog_->Show(false); }
    void WxDialogPeer::destroy() { if (dialog_ == nullptr) return; auto* value = dialog_; dialog_ = nullptr; if (value->IsModal()) value->EndModal(wxID_CANCEL); value->Destroy(); }
    void WxDialogPeer::toFront() { if (dialog_ != nullptr) dialog_->Raise(); }
    void WxDialogPeer::toBack() { if (dialog_ != nullptr) dialog_->Lower(); }
    void WxDialogPeer::setBounds(::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint width, ::jxx::lang::jint height) { if (dialog_ != nullptr) dialog_->SetSize(x, y, width, height); }
    void WxDialogPeer::setTitle(const ::jxx::Ptr<::jxx::lang::String>& title) { title_ = title != nullptr ? title : ::jxx::NEW<::jxx::lang::String>(""); if (dialog_ != nullptr) dialog_->SetTitle(nativeText(title_)); }
    ::jxx::Ptr<::jxx::lang::String> WxDialogPeer::getTitle() const { return title_; }
    ::jxx::lang::jbool WxDialogPeer::isShown() const { return dialog_ != nullptr && dialog_->IsShown(); }
    void WxDialogPeer::setMenuBar(const ::jxx::Ptr<::jxx::awt::MenuBar>&) {}
    void WxDialogPeer::setResizable(::jxx::lang::jbool resizable)
    {
        if (dialog_ == nullptr) return;
        auto style = dialog_->GetWindowStyleFlag();
        if (resizable) style |= wxRESIZE_BORDER;
        else style &= ~wxRESIZE_BORDER;
        dialog_->SetWindowStyleFlag(style);
    }
    ::jxx::lang::jbool WxDialogPeer::isModal() const { return dialog_ != nullptr && dialog_->IsModal(); }
    void WxDialogPeer::installComponents(const ::jxx::Ptr<::jxx::awt::Container>& container) { if (dialog_ != nullptr && container != nullptr) buildChildren(dialog_, container); }
    void WxDialogPeer::buildChildren(wxWindow* parent, const ::jxx::Ptr<::jxx::awt::Container>& container)
    {
        for (::jxx::lang::jint index = 0; index < container->getComponentCount(); ++index)
        {
            const auto component = container->getComponent(index);
            if (component == nullptr || component->nativeComponent_ != nullptr) continue;
            wxWindow* native = nullptr;
            if (const auto button = ::jxx::CAST<::jxx::swing::AbstractButton>(component))
            {
                auto* control = new wxButton(parent, wxID_ANY, nativeText(button->getText()));
                const auto peer = ::jxx::NEW<WxComponentPeer>(control);
                std::weak_ptr<::jxx::swing::AbstractButton> weak = button;
                peer->setActionCallback([weak] { if (const auto owner = weak.lock()) owner->fireActionPerformed(); });
                component->setNativeComponentInternal(peer); native = control;
            }
            else if (const auto label = ::jxx::CAST<::jxx::swing::JLabel>(component))
            {
                auto* control = new wxStaticText(parent, wxID_ANY, nativeText(label->getText()));
                component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(control)); native = control;
            }
            else if (const auto button = ::jxx::CAST<::jxx::awt::Button>(component))
            {
                auto* control = new wxButton(parent, wxID_ANY, nativeText(button->getLabel()));
                const auto peer = ::jxx::NEW<WxComponentPeer>(control);
                std::weak_ptr<::jxx::awt::Button> weak = button;
                peer->setActionCallback([weak] { if (const auto owner = weak.lock()) owner->fireActionPerformed(); });
                component->setNativeComponentInternal(peer); native = control;
            }
            else if (const auto label = ::jxx::CAST<::jxx::awt::Label>(component))
            {
                auto* control = new wxStaticText(parent, wxID_ANY, nativeText(label->getText()));
                component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(control)); native = control;
            }
            else if (const auto field = ::jxx::CAST<::jxx::awt::TextField>(component))
            {
                auto* control = new wxTextCtrl(parent, wxID_ANY, nativeText(field->getText()), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
                const auto peer = ::jxx::NEW<WxComponentPeer>(control);
                std::weak_ptr<::jxx::awt::TextField> weak = field;
                peer->setActionCallback([weak] { if (const auto owner = weak.lock()) owner->fireActionPerformed(); });
                component->setNativeComponentInternal(peer); native = control;
            }
            else if (const auto child = ::jxx::CAST<::jxx::awt::Container>(component))
            {
                auto* panel = new wxPanel(parent, wxID_ANY);
                component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(panel)); native = panel; buildChildren(panel, child);
            }
            if (native != nullptr) native->Show(component->isVisible());
        }
    }
}
