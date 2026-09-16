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
        if (window_ != nullptr)
        {
            window_->Bind(wxEVT_DESTROY, [this](wxWindowDestroyEvent& event)
            {
                nativeDestroyed();
                event.Skip();
            });
        }

        if (window_ != nullptr)
        {
            window_->Bind(wxEVT_SET_FOCUS,[this](wxFocusEvent& event){if(focusCallback_)focusCallback_(true);event.Skip();});
            window_->Bind(wxEVT_KILL_FOCUS,[this](wxFocusEvent& event){if(focusCallback_)focusCallback_(false);event.Skip();});
            window_->Bind(wxEVT_KEY_DOWN,[this](wxKeyEvent& event){if(keyCallback_)keyCallback_(401,event.GetKeyCode(),static_cast<::jxx::lang::jchar>(event.GetUnicodeKey()),event.GetModifiers());event.Skip();});
            window_->Bind(wxEVT_KEY_UP,[this](wxKeyEvent& event){if(keyCallback_)keyCallback_(402,event.GetKeyCode(),static_cast<::jxx::lang::jchar>(event.GetUnicodeKey()),event.GetModifiers());event.Skip();});
            auto mouseHandler = [this](wxMouseEvent& event)
            {
                if (mouseCallback_)
                {
                    const int id = event.ButtonDClick() ? 500
                        : event.ButtonDown() ? 501
                        : event.ButtonUp() ? 502
                        : event.Dragging() ? 506
                        : event.Moving() ? 503
                        : event.Entering() ? 504
                        : event.Leaving() ? 505
                        : 0;
                    const int button = event.GetButton() == wxMOUSE_BTN_LEFT ? 1
                        : event.GetButton() == wxMOUSE_BTN_MIDDLE ? 2
                        : event.GetButton() == wxMOUSE_BTN_RIGHT ? 3
                        : 0;
                    if (id != 0)
                        mouseCallback_(id, event.GetX(), event.GetY(), button,
                            event.GetClickCount(), event.RightDown() || event.RightUp());
                }
                event.Skip();
            };
            window_->Bind(wxEVT_LEFT_DOWN, mouseHandler);
            window_->Bind(wxEVT_LEFT_UP, mouseHandler);
            window_->Bind(wxEVT_LEFT_DCLICK, mouseHandler);
            window_->Bind(wxEVT_MIDDLE_DOWN, mouseHandler);
            window_->Bind(wxEVT_MIDDLE_UP, mouseHandler);
            window_->Bind(wxEVT_MIDDLE_DCLICK, mouseHandler);
            window_->Bind(wxEVT_RIGHT_DOWN, mouseHandler);
            window_->Bind(wxEVT_RIGHT_UP, mouseHandler);
            window_->Bind(wxEVT_RIGHT_DCLICK, mouseHandler);
            window_->Bind(wxEVT_MOTION, mouseHandler);
            window_->Bind(wxEVT_ENTER_WINDOW, mouseHandler);
            window_->Bind(wxEVT_LEAVE_WINDOW, mouseHandler);
            window_->Bind(wxEVT_MOUSEWHEEL, [this](wxMouseEvent& event) { if (mouseWheelCallback_) { const int delta = event.GetWheelDelta(); const int rotation = delta != 0 ? event.GetWheelRotation() / delta : 0; const double precise = delta != 0 ? static_cast<double>(event.GetWheelRotation()) / static_cast<double>(delta) : 0.0; mouseWheelCallback_(event.GetLinesPerAction(), rotation, event.GetWheelAxis(), precise); } event.Skip(); });
            window_->Bind(wxEVT_CHAR,[this](wxKeyEvent& event){if(keyCallback_)keyCallback_(400,0,static_cast<::jxx::lang::jchar>(event.GetUnicodeKey()),event.GetModifiers());event.Skip();});
        }
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
            text->Bind(wxEVT_TEXT, [this, text](wxCommandEvent& event) { if (textCallback_) textCallback_(::jxx::NEW<::jxx::lang::String>(text->GetValue().ToUTF8().data())); event.Skip(); });
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

    void WxComponentPeer::nativeDestroyed()
    {
        window_ = nullptr;
        actionCallback_ = {};
        focusCallback_ = {};
        keyCallback_ = {};
        mouseCallback_ = {};
        mouseWheelCallback_ = {};
        textCallback_ = {};
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

    void WxComponentPeer::setTextCallback(TextCallback callback){textCallback_=std::move(callback);}
    void WxComponentPeer::setMouseWheelCallback(MouseWheelCallback callback){mouseWheelCallback_=std::move(callback);}
    void WxComponentPeer::setMouseCallback(MouseCallback callback){mouseCallback_=std::move(callback);}
    void WxComponentPeer::setKeyCallback(KeyCallback callback){keyCallback_=std::move(callback);}
    void WxComponentPeer::requestFocus(){if(window_!=nullptr)window_->SetFocus();}
    ::jxx::lang::jbool WxComponentPeer::hasFocus() const{return window_!=nullptr&&window_->HasFocus();}
    void WxComponentPeer::setFocusCallback(FocusCallback callback){focusCallback_=std::move(callback);}

    void WxComponentPeer::setActionCallback(ActionCallback callback)
    {
        actionCallback_ = std::move(callback);
    }

    wxWindow* WxComponentPeer::nativeWindow() const
    {
        return window_;
    }
}
