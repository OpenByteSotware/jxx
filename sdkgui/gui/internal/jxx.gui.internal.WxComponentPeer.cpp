#include "gui/internal/jxx.gui.internal.WxComponentPeer.h"
#include <utility>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/window.h>
namespace jxx::gui::internal {
WxComponentPeer::WxComponentPeer(wxWindow* window):window_(window){if(auto* button=dynamic_cast<wxButton*>(window_))button->Bind(wxEVT_BUTTON,[this](wxCommandEvent& event){if(actionCallback_)actionCallback_();event.Skip();});if(auto* text=dynamic_cast<wxTextCtrl*>(window_))text->Bind(wxEVT_TEXT_ENTER,[this](wxCommandEvent& event){if(actionCallback_)actionCallback_();event.Skip();});}
WxComponentPeer::~WxComponentPeer(){destroy();} void WxComponentPeer::destroy(){if(window_){auto* value=window_;window_=nullptr;value->Destroy();}}
void WxComponentPeer::setVisible(::jxx::lang::jbool value){if(window_)window_->Show(value);} void WxComponentPeer::setEnabled(::jxx::lang::jbool value){if(window_)window_->Enable(value);}
void WxComponentPeer::setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height){if(window_)window_->SetSize(x,y,width,height);}
void WxComponentPeer::setText(const ::jxx::Ptr<::jxx::lang::String>& value){const auto text=value ? wxString::FromUTF8(value->utf8().c_str()) : wxString();if(auto* control=dynamic_cast<wxControl*>(window_))control->SetLabel(text);if(auto* entry=dynamic_cast<wxTextCtrl*>(window_))entry->ChangeValue(text);}
::jxx::Ptr<::jxx::lang::String> WxComponentPeer::getText() const{wxString value;if(auto* entry=dynamic_cast<wxTextCtrl*>(window_))value=entry->GetValue();else if(auto* control=dynamic_cast<wxControl*>(window_))value=control->GetLabel();return ::jxx::NEW<::jxx::lang::String>(value.ToUTF8().data());}
void WxComponentPeer::setActionCallback(ActionCallback callback){actionCallback_=std::move(callback);} wxWindow* WxComponentPeer::nativeWindow() const{return window_;}
}
