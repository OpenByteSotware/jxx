#include "awt/jxx.awt.Frame.h"
#include "awt/jxx.awt.MenuBar.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"
#include "gui/internal/jxx.gui.internal.WxFramePeer.h"
namespace jxx::awt {
Frame::Frame():title_(::jxx::NEW<::jxx::lang::String>("")){} Frame::Frame(const ::jxx::Ptr<::jxx::lang::String>& title):title_(title ? title : ::jxx::NEW<::jxx::lang::String>("")){}
void Frame::ensureNativeWindow(){if(!nativeWindow_){nativeWindow_=::jxx::NEW<::jxx::gui::internal::WxFramePeer>(title_);nativeWindow_->setBounds(getX(),getY(),getWidth(),getHeight());nativeWindow_->setMenuBar(menuBar_);configureNativeEvents();displayable_=true;}}
void Frame::setTitle(const ::jxx::Ptr<::jxx::lang::String>& title){title_=title ? title : ::jxx::NEW<::jxx::lang::String>("");if(nativeWindow_)nativeWindow_->setTitle(title_);} ::jxx::Ptr<::jxx::lang::String> Frame::getTitle() const{return title_;}
::jxx::Ptr<MenuBar> Frame::getMenuBar() const{return menuBar_;} void Frame::setMenuBar(const ::jxx::Ptr<MenuBar>& value){menuBar_=value;if(nativeWindow_)nativeWindow_->setMenuBar(menuBar_);} void Frame::remove(const ::jxx::Ptr<MenuComponent>& component){if(menuBar_&&component==::jxx::CAST<MenuComponent>(menuBar_)){menuBar_.reset();if(nativeWindow_)nativeWindow_->setMenuBar(nullptr);}}
}
