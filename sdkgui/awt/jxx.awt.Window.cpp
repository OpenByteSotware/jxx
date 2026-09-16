#include "awt/jxx.awt.Window.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "awt/event/jxx.awt.event.WindowListener.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"
namespace jxx::awt {
Window::Window():active_(false),displayable_(false){Component::setVisible(false);} Window::~Window(){dispose();}
void Window::ensureNativeWindow(){}
void Window::configureNativeEvents(){if(!nativeWindow_)return;std::weak_ptr<Window> self=::jxx::CAST<Window>(thisPtr());nativeWindow_->setEventCallback([self](::jxx::lang::jint id){if(auto owner=self.lock())owner->processWindowEvent(::jxx::NEW<::jxx::awt::event::WindowEvent>(owner,id));});}
void Window::addWindowListener(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& l){if(l&&std::find(windowListeners_.begin(),windowListeners_.end(),l)==windowListeners_.end())windowListeners_.push_back(l);}
void Window::removeWindowListener(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& l){windowListeners_.erase(std::remove(windowListeners_.begin(),windowListeners_.end(),l),windowListeners_.end());}
void Window::processWindowEvent(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& e){if(!e)return;const auto id=e->getID();if(id==::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED)active_=true;else if(id==::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED)active_=false;else if(id==::jxx::awt::event::WindowEvent::WINDOW_CLOSED){active_=false;displayable_=false;Component::setVisible(false);}auto copy=windowListeners_;for(const auto& l:copy){if(!l)continue;switch(id){case ::jxx::awt::event::WindowEvent::WINDOW_OPENED:l->windowOpened(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_CLOSING:l->windowClosing(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_CLOSED:l->windowClosed(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_ICONIFIED:l->windowIconified(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_DEICONIFIED:l->windowDeiconified(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED:l->windowActivated(e);break;case ::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED:l->windowDeactivated(e);break;}}}
void Window::setVisible(::jxx::lang::jbool v){if(v)ensureNativeWindow();Component::setVisible(v);if(nativeWindow_){if(v){nativeWindow_->setBounds(getX(),getY(),getWidth(),getHeight());nativeWindow_->show();displayable_=true;}else nativeWindow_->hide();}}
void Window::setBounds(const ::jxx::Ptr<Rectangle>& r){if(r)setBounds(r->x,r->y,r->width,r->height);} void Window::setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint w,::jxx::lang::jint h){Component::setBounds(x,y,w,h);if(nativeWindow_)nativeWindow_->setBounds(x,y,w,h);}
void Window::pack(){validate();ensureNativeWindow();displayable_=nativeWindow_!=nullptr;} void Window::dispose(){if(nativeWindow_){nativeWindow_->setEventCallback({});nativeWindow_->destroy();nativeWindow_.reset();}active_=false;displayable_=false;Component::setVisible(false);}
void Window::toFront(){ensureNativeWindow();if(nativeWindow_)nativeWindow_->toFront();} void Window::toBack(){if(nativeWindow_)nativeWindow_->toBack();} ::jxx::lang::jbool Window::isActive() const{return active_;} ::jxx::lang::jbool Window::isDisplayable() const{return displayable_;}
}
