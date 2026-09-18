#include "swing/jxx.swing.JDesktopPane.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JInternalFrame.h"
namespace jxx::swing
{
    JDesktopPane::JDesktopPane():Super(){}
    ::jxx::Ptr<::jxx::awt::Component> JDesktopPane::add(const ::jxx::Ptr<::jxx::awt::Component>& c){auto result=JLayeredPane::add(c);if(auto f=::jxx::CAST<JInternalFrame>(c))frames_.push_back(f);return result;}
    void JDesktopPane::remove(const ::jxx::Ptr<::jxx::awt::Component>& c){if(auto f=::jxx::CAST<JInternalFrame>(c)){frames_.erase(std::remove(frames_.begin(),frames_.end(),f),frames_.end());if(selected_==f)selected_.reset();}JLayeredPane::remove(c);}
    std::vector<::jxx::Ptr<JInternalFrame>> JDesktopPane::getAllFrames()const{return frames_;}::jxx::Ptr<JInternalFrame> JDesktopPane::getSelectedFrame()const{return selected_;}void JDesktopPane::setSelectedFrame(const ::jxx::Ptr<JInternalFrame>& f){if(selected_&&selected_!=f)selected_->setSelected(false);selected_=f;if(selected_)selected_->setSelected(true);}void JDesktopPane::setDragMode(::jxx::lang::jint v){if(v!=LIVE_DRAG_MODE&&v!=OUTLINE_DRAG_MODE)throw ::jxx::lang::IllegalArgumentException("mode");dragMode_=v;}::jxx::lang::jint JDesktopPane::getDragMode()const{return dragMode_;}
}
