#include "gui/internal/jxx.gui.internal.WxComponentPeer.h"

#include <utility>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/control.h>
#include <wx/font.h>
#include <wx/textctrl.h>
#include <wx/window.h>

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Font.h"

namespace jxx::gui::internal
{
    WxComponentPeer::WxComponentPeer(wxWindow* window)
        : window_(window)
    {
        if (auto* button = dynamic_cast<wxButton*>(window_))
        {
            button->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
            {
                if (actionCallback_) actionCallback_();
                event.Skip();
            });
        }

        if (auto* text = dynamic_cast<wxTextCtrl*>(window_))
        {
            text->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event)
            {
                if (actionCallback_) actionCallback_();
                event.Skip();
            });
        }
    }

    WxComponentPeer::~WxComponentPeer()
    {
        destroy();
    }

    void WxComponentPeer::destroy()
    {
        if (window_ != nullptr)
        {
            auto* value = window_;
            window_ = nullptr;
            value->Destroy();
        }
    }

    void WxComponentPeer::setVisible(::jxx::lang::jbool value)
    {
        if (window_ != nullptr) window_->Show(value);
    }

    void WxComponentPeer::setEnabled(::jxx::lang::jbool value)
    {
        if (window_ != nullptr) window_->Enable(value);
    }

    void WxComponentPeer::setBounds(
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        if (window_ != nullptr) window_->SetSize(x, y, width, height);
    }

    void WxComponentPeer::setText(
        const ::jxx::Ptr<::jxx::lang::String>& value)
    {
        const auto text = value != nullptr
            ? wxString::FromUTF8(value->utf8().c_str())
            : wxString();

        if (auto* entry = dynamic_cast<wxTextCtrl*>(window_))
            entry->ChangeValue(text);
        else if (auto* control = dynamic_cast<wxControl*>(window_))
            control->SetLabel(text);
    }

    ::jxx::Ptr<::jxx::lang::String> WxComponentPeer::getText() const
    {
        wxString value;
        if (auto* entry = dynamic_cast<wxTextCtrl*>(window_))
            value = entry->GetValue();
        else if (auto* control = dynamic_cast<wxControl*>(window_))
            value = control->GetLabel();

        return ::jxx::NEW<::jxx::lang::String>(value.ToUTF8().data());
    }

    void WxComponentPeer::setEditable(::jxx::lang::jbool value)
    {
        if (auto* entry = dynamic_cast<wxTextCtrl*>(window_))
            entry->SetEditable(value);
    }

    void WxComponentPeer::setSelection(
        ::jxx::lang::jint start,
        ::jxx::lang::jint end)
    {
        if (auto* entry = dynamic_cast<wxTextCtrl*>(window_))
            entry->SetSelection(start, end);
    }

    void WxComponentPeer::setCaretPosition(::jxx::lang::jint position)
    {
        if (auto* entry = dynamic_cast<wxTextCtrl*>(window_))
            entry->SetInsertionPoint(position);
    }

    void WxComponentPeer::setForeground(
        const ::jxx::Ptr<::jxx::awt::Color>& color)
    {
        if (window_ == nullptr || color == nullptr) return;
        window_->SetForegroundColour(wxColour(
            color->getRed(), color->getGreen(), color->getBlue(), color->getAlpha()));
        window_->Refresh();
    }

    void WxComponentPeer::setBackground(
        const ::jxx::Ptr<::jxx::awt::Color>& color)
    {
        if (window_ == nullptr || color == nullptr) return;
        window_->SetBackgroundColour(wxColour(
            color->getRed(), color->getGreen(), color->getBlue(), color->getAlpha()));
        window_->Refresh();
    }

    void WxComponentPeer::setFont(
        const ::jxx::Ptr<::jxx::awt::Font>& font)
    {
        if (window_ == nullptr || font == nullptr) return;

        wxFontInfo info(font->getSize());
        info.FaceName(wxString::FromUTF8(font->getName()->utf8().c_str()));
        info.Bold(font->isBold());
        info.Italic(font->isItalic());
        window_->SetFont(wxFont(info));
        window_->Refresh();
    }

    void WxComponentPeer::setActionCallback(ActionCallback callback)
    {
        actionCallback_ = std::move(callback);
    }

    wxWindow* WxComponentPeer::nativeWindow() const
    {
        return window_;
    }
}
