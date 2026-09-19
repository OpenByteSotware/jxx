#include "swing/jxx.swing.JInternalFrame.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.InternalFrameEvent.h"
#include "swing/event/jxx.swing.event.InternalFrameListener.h"
#include "swing/jxx.swing.JLayeredPane.h"
#include "swing/jxx.swing.JRootPane.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyTitle(){ return ::jxx::NEW<::jxx::lang::String>(""); } }
    JInternalFrame::JInternalFrame():JInternalFrame(emptyTitle()){} JInternalFrame::JInternalFrame(const ::jxx::Ptr<::jxx::lang::String>& t):JInternalFrame(t,false,false,false,false){}
    JInternalFrame::JInternalFrame(const ::jxx::Ptr<::jxx::lang::String>& t,::jxx::lang::jbool r,::jxx::lang::jbool c,::jxx::lang::jbool m,::jxx::lang::jbool i):Super(),rootPane_(::jxx::NEW<JRootPane>()),title_(t?t:emptyTitle()),resizable_(r),closable_(c),maximizable_(m),iconifiable_(i){JComponent::add(rootPane_);}
    ::jxx::Ptr<JRootPane> JInternalFrame::getRootPane()const{return rootPane_;}void JInternalFrame::setContentPane(const ::jxx::Ptr<::jxx::awt::Container>& p){rootPane_->setContentPane(p);}::jxx::Ptr<::jxx::awt::Container> JInternalFrame::getContentPane()const{return rootPane_->getContentPane();}void JInternalFrame::setLayeredPane(const ::jxx::Ptr<JLayeredPane>& p){rootPane_->setLayeredPane(p);}::jxx::Ptr<JLayeredPane> JInternalFrame::getLayeredPane()const{return rootPane_->getLayeredPane();}void JInternalFrame::setGlassPane(const ::jxx::Ptr<::jxx::awt::Component>& p){rootPane_->setGlassPane(p);}::jxx::Ptr<::jxx::awt::Component> JInternalFrame::getGlassPane()const{return rootPane_->getGlassPane();}
    void JInternalFrame::setTitle(const ::jxx::Ptr<::jxx::lang::String>& v){title_=v?v:emptyTitle();}::jxx::Ptr<::jxx::lang::String> JInternalFrame::getTitle()const{return title_;}void JInternalFrame::setClosed(::jxx::lang::jbool v){
        if (closed_ == v) return;
        if (v) processInternalFrameEvent(::jxx::NEW<::jxx::swing::event::InternalFrameEvent>(::jxx::CAST<JInternalFrame>(thisPtr()), ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_CLOSING));
        closed_=v;
        if(v){setVisible(false);processInternalFrameEvent(::jxx::NEW<::jxx::swing::event::InternalFrameEvent>(::jxx::CAST<JInternalFrame>(thisPtr()), ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_CLOSED));}
    }::jxx::lang::jbool JInternalFrame::isClosed()const{return closed_;}void JInternalFrame::setIcon(::jxx::lang::jbool v){
        if(icon_==v)return; icon_=v;
        processInternalFrameEvent(::jxx::NEW<::jxx::swing::event::InternalFrameEvent>(::jxx::CAST<JInternalFrame>(thisPtr()), v ? ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_ICONIFIED : ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_DEICONIFIED));
    }::jxx::lang::jbool JInternalFrame::isIcon()const{return icon_;}void JInternalFrame::setMaximum(::jxx::lang::jbool v){maximum_=v;}::jxx::lang::jbool JInternalFrame::isMaximum()const{return maximum_;}void JInternalFrame::setSelected(::jxx::lang::jbool v){
        if(selected_==v)return; selected_=v;
        processInternalFrameEvent(::jxx::NEW<::jxx::swing::event::InternalFrameEvent>(::jxx::CAST<JInternalFrame>(thisPtr()), v ? ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_ACTIVATED : ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_DEACTIVATED));
    }::jxx::lang::jbool JInternalFrame::isSelected()const{return selected_;}
    ::jxx::lang::jbool JInternalFrame::isResizable()const{return resizable_;}::jxx::lang::jbool JInternalFrame::isClosable()const{return closable_;}::jxx::lang::jbool JInternalFrame::isMaximizable()const{return maximizable_;}::jxx::lang::jbool JInternalFrame::isIconifiable()const{return iconifiable_;}void JInternalFrame::addInternalFrameListener(const ::jxx::Ptr<::jxx::swing::event::InternalFrameListener>& listener){if(listener&&std::find(internalFrameListeners_.begin(),internalFrameListeners_.end(),listener)==internalFrameListeners_.end())internalFrameListeners_.push_back(listener);}
    void JInternalFrame::removeInternalFrameListener(const ::jxx::Ptr<::jxx::swing::event::InternalFrameListener>& listener){internalFrameListeners_.erase(std::remove(internalFrameListeners_.begin(),internalFrameListeners_.end(),listener),internalFrameListeners_.end());}
    void JInternalFrame::processInternalFrameEvent(const ::jxx::Ptr<::jxx::swing::event::InternalFrameEvent>& event){const auto copy=internalFrameListeners_;for(const auto& listener:copy){if(!listener)continue;switch(event->getID()){case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_OPENED:listener->internalFrameOpened(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_CLOSING:listener->internalFrameClosing(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_CLOSED:listener->internalFrameClosed(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_ICONIFIED:listener->internalFrameIconified(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_DEICONIFIED:listener->internalFrameDeiconified(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_ACTIVATED:listener->internalFrameActivated(event);break;case ::jxx::swing::event::InternalFrameEvent::INTERNAL_FRAME_DEACTIVATED:listener->internalFrameDeactivated(event);break;default:break;}}}
    void JInternalFrame::dispose(){setClosed(true);}void JInternalFrame::moveToFront(){if(auto p=::jxx::CAST<JLayeredPane>(getParent()))p->moveToFront(::jxx::CAST<::jxx::awt::Component>(thisPtr()));}void JInternalFrame::moveToBack(){if(auto p=::jxx::CAST<JLayeredPane>(getParent()))p->moveToBack(::jxx::CAST<::jxx::awt::Component>(thisPtr()));}
}
