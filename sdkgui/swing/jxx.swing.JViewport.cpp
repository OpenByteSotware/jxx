#include "swing/jxx.swing.JViewport.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    JViewport::JViewport():Super(),position_(::jxx::NEW<::jxx::awt::Point>()),viewSize_(::jxx::NEW<::jxx::awt::Dimension>()){setOpaque(true);setLayout(nullptr);}
    void JViewport::setView(const ::jxx::Ptr<::jxx::awt::Component>& v){removeAll();view_=v;if(view_)Super::add(view_);}
    ::jxx::Ptr<::jxx::awt::Component> JViewport::getView()const{return view_;}
    void JViewport::setViewPosition(const ::jxx::Ptr<::jxx::awt::Point>& p){if(!p)return;position_=::jxx::NEW<::jxx::awt::Point>(p);if(view_)view_->setLocation(-p->x,-p->y);}
    ::jxx::Ptr<::jxx::awt::Point> JViewport::getViewPosition()const{return ::jxx::NEW<::jxx::awt::Point>(position_);}
    void JViewport::setViewSize(const ::jxx::Ptr<::jxx::awt::Dimension>& s){if(!s)return;viewSize_=::jxx::NEW<::jxx::awt::Dimension>(s);if(view_)view_->setSize(s);}
    ::jxx::Ptr<::jxx::awt::Dimension> JViewport::getViewSize()const{return view_?view_->getSize(): ::jxx::NEW<::jxx::awt::Dimension>(viewSize_);}
    void JViewport::setScrollMode(::jxx::lang::jint m){if(m<0||m>2)throw ::jxx::lang::IllegalArgumentException("mode");scrollMode_=m;} ::jxx::lang::jint JViewport::getScrollMode()const{return scrollMode_;}
}
