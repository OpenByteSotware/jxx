#include "gui/internal/jxx.gui.internal.WxFramePeer.h"
#include <utility>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/string.h>
#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "awt/jxx.awt.CheckboxMenuItem.h"
#include "awt/jxx.awt.Menu.h"
#include "awt/jxx.awt.MenuBar.h"
#include "awt/jxx.awt.MenuItem.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
namespace jxx::gui::internal {
namespace { wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& v){return v ? wxString::FromUTF8(v->utf8().c_str()) : wxString();} }
WxFramePeer::WxFramePeer(const ::jxx::Ptr<::jxx::lang::String>& title):frame_(nullptr),title_(title ? title : ::jxx::NEW<::jxx::lang::String>("")){
 if(GuiRuntime::ensureInitialized()){
  frame_=new wxFrame(nullptr,wxID_ANY,nativeText(title_),wxDefaultPosition,wxDefaultSize,wxDEFAULT_FRAME_STYLE);
  frame_->Bind(wxEVT_SHOW,[this](wxShowEvent& e){send(e.IsShown() ? ::jxx::awt::event::WindowEvent::WINDOW_OPENED : ::jxx::awt::event::WindowEvent::WINDOW_CLOSED);e.Skip();});
  frame_->Bind(wxEVT_ACTIVATE,[this](wxActivateEvent& e){send(e.GetActive() ? ::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED : ::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED);e.Skip();});
  frame_->Bind(wxEVT_ICONIZE,[this](wxIconizeEvent& e){send(e.IsIconized() ? ::jxx::awt::event::WindowEvent::WINDOW_ICONIFIED : ::jxx::awt::event::WindowEvent::WINDOW_DEICONIFIED);e.Skip();});
  frame_->Bind(wxEVT_CLOSE_WINDOW,[this](wxCloseEvent& e){send(::jxx::awt::event::WindowEvent::WINDOW_CLOSING);e.Skip();});
  frame_->Bind(wxEVT_DESTROY,[this](wxWindowDestroyEvent& e){send(::jxx::awt::event::WindowEvent::WINDOW_CLOSED);frame_=nullptr;e.Skip();});
  frame_->Bind(wxEVT_MENU,[this](wxCommandEvent& e){dispatchMenu(e.GetId());});
 }
}
WxFramePeer::~WxFramePeer(){destroy();} void WxFramePeer::send(::jxx::lang::jint id){if(callback_)callback_(id);} void WxFramePeer::setEventCallback(EventCallback cb){callback_=std::move(cb);}
void WxFramePeer::show(){if(frame_)frame_->Show(true);} void WxFramePeer::hide(){if(frame_)frame_->Show(false);} void WxFramePeer::destroy(){menuItems_.clear();if(frame_){auto* value=frame_;frame_=nullptr;value->Destroy();}}
void WxFramePeer::toFront(){if(frame_)frame_->Raise();} void WxFramePeer::toBack(){if(frame_)frame_->Lower();} void WxFramePeer::setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint w,::jxx::lang::jint h){if(frame_)frame_->SetSize(x,y,w,h);}
void WxFramePeer::setTitle(const ::jxx::Ptr<::jxx::lang::String>& title){title_=title ? title : ::jxx::NEW<::jxx::lang::String>("");if(frame_)frame_->SetTitle(nativeText(title_));} ::jxx::Ptr<::jxx::lang::String> WxFramePeer::getTitle() const{return title_;} ::jxx::lang::jbool WxFramePeer::isShown() const{return frame_&&frame_->IsShown();}
wxMenu* WxFramePeer::buildMenu(const ::jxx::Ptr<::jxx::awt::Menu>& menu){auto* nativeMenu=new wxMenu();if(!menu)return nativeMenu;for(::jxx::lang::jint i=0;i<menu->getItemCount();++i){auto item=menu->getItem(i);if(!item)continue;const auto label=item->getLabel();if(label&&label->utf8()=="-"){nativeMenu->AppendSeparator();continue;}if(auto submenu=::jxx::CAST<::jxx::awt::Menu>(item)){nativeMenu->AppendSubMenu(buildMenu(submenu),nativeText(submenu->getLabel()));continue;}const int id=wxWindow::NewControlId();wxItemKind kind=::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item) ? wxITEM_CHECK : wxITEM_NORMAL;auto* nativeItem=nativeMenu->Append(id,nativeText(label),wxEmptyString,kind);nativeItem->Enable(item->isEnabled());if(auto check=::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item))nativeItem->Check(check->getState());menuItems_[id]=item;}return nativeMenu;}
void WxFramePeer::setMenuBar(const ::jxx::Ptr<::jxx::awt::MenuBar>& menuBar){if(!frame_)return;menuItems_.clear();auto* nativeBar=new wxMenuBar();if(menuBar){for(::jxx::lang::jint i=0;i<menuBar->getMenuCount();++i){auto menu=menuBar->getMenu(i);if(menu)nativeBar->Append(buildMenu(menu),nativeText(menu->getLabel()));}}frame_->SetMenuBar(nativeBar);}
void WxFramePeer::dispatchMenu(::jxx::lang::jint id){auto it=menuItems_.find(id);if(it==menuItems_.end())return;if(auto item=it->second.lock()){if(auto check=::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item))check->setState(!check->getState());item->fireActionPerformed();}}
}
