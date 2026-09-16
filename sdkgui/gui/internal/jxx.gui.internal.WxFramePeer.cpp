#include "gui/internal/jxx.gui.internal.WxFramePeer.h"
#include <utility>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/string.h>
#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
namespace jxx::gui::internal {
namespace { wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& v){return v?wxString::FromUTF8(v->utf8().c_str()):wxString();} }
WxFramePeer::WxFramePeer(const ::jxx::Ptr<::jxx::lang::String>& title):frame_(nullptr),title_(title? title : ::jxx::NEW<::jxx::lang::String>("")){
 if(GuiRuntime::ensureInitialized()){
  frame_=new wxFrame(nullptr,wxID_ANY,nativeText(title_),wxDefaultPosition,wxDefaultSize,wxDEFAULT_FRAME_STYLE);
  frame_->Bind(wxEVT_SHOW,[this](wxShowEvent& e){send(e.IsShown()?::jxx::awt::event::WindowEvent::WINDOW_OPENED: ::jxx::awt::event::WindowEvent::WINDOW_CLOSED);e.Skip();});
  frame_->Bind(wxEVT_ACTIVATE,[this](wxActivateEvent& e){send(e.GetActive()?::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED: ::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED);e.Skip();});
  frame_->Bind(wxEVT_ICONIZE,[this](wxIconizeEvent& e){send(e.IsIconized()?::jxx::awt::event::WindowEvent::WINDOW_ICONIFIED: ::jxx::awt::event::WindowEvent::WINDOW_DEICONIFIED);e.Skip();});
  frame_->Bind(wxEVT_CLOSE_WINDOW,[this](wxCloseEvent& e){send(::jxx::awt::event::WindowEvent::WINDOW_CLOSING);e.Skip();});
  frame_->Bind(wxEVT_DESTROY,[this](wxWindowDestroyEvent& e){send(::jxx::awt::event::WindowEvent::WINDOW_CLOSED);frame_=nullptr;e.Skip();});
 }
}
WxFramePeer::~WxFramePeer(){destroy();}
void WxFramePeer::send(::jxx::lang::jint id){if(callback_)callback_(id);}
void WxFramePeer::setEventCallback(EventCallback callback){callback_=std::move(callback);}
void WxFramePeer::show(){if(frame_)frame_->Show(true);} void WxFramePeer::hide(){if(frame_)frame_->Show(false);}
void WxFramePeer::destroy(){if(frame_){auto* value=frame_;frame_=nullptr;value->Destroy();}}
void WxFramePeer::toFront(){if(frame_)frame_->Raise();} void WxFramePeer::toBack(){if(frame_)frame_->Lower();}
void WxFramePeer::setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint w,::jxx::lang::jint h){if(frame_)frame_->SetSize(x,y,w,h);}
void WxFramePeer::setTitle(const ::jxx::Ptr<::jxx::lang::String>& title){title_=title? title : ::jxx::NEW<::jxx::lang::String>("");if(frame_)frame_->SetTitle(nativeText(title_));}
::jxx::Ptr<::jxx::lang::String> WxFramePeer::getTitle() const{return title_;} ::jxx::lang::jbool WxFramePeer::isShown() const{return frame_&&frame_->IsShown();}
}
