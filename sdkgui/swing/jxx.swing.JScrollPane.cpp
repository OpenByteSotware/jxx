#include "swing/jxx.swing.JScrollPane.h"
#include "swing/jxx.swing.JScrollBar.h"
#include "swing/jxx.swing.JViewport.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    JScrollPane::JScrollPane():JScrollPane(nullptr){}
    JScrollPane::JScrollPane(const ::jxx::Ptr<::jxx::awt::Component>& v):JScrollPane(v,VERTICAL_SCROLLBAR_AS_NEEDED,HORIZONTAL_SCROLLBAR_AS_NEEDED){}
    JScrollPane::JScrollPane(const ::jxx::Ptr<::jxx::awt::Component>& v,::jxx::lang::jint vp,::jxx::lang::jint hp):Super(),vertical_(::jxx::NEW<JScrollBar>(::jxx::awt::Adjustable::VERTICAL)),horizontal_(::jxx::NEW<JScrollBar>(::jxx::awt::Adjustable::HORIZONTAL)){setLayout(nullptr);setVerticalScrollBarPolicy(vp);setHorizontalScrollBarPolicy(hp);setViewport(::jxx::NEW<JViewport>());setViewportView(v);}
    void JScrollPane::setViewport(const ::jxx::Ptr<JViewport>& v){if(viewport_)Super::remove(viewport_);viewport_=v;if(viewport_)Super::add(viewport_);}
    ::jxx::Ptr<JViewport> JScrollPane::getViewport()const{return viewport_;} void JScrollPane::setViewportView(const ::jxx::Ptr<::jxx::awt::Component>& v){if(viewport_)viewport_->setView(v);}
    void JScrollPane::setVerticalScrollBarPolicy(::jxx::lang::jint p){if(p<20||p>22)throw ::jxx::lang::IllegalArgumentException("policy");verticalPolicy_=p;} ::jxx::lang::jint JScrollPane::getVerticalScrollBarPolicy()const{return verticalPolicy_;}
    void JScrollPane::setHorizontalScrollBarPolicy(::jxx::lang::jint p){if(p<30||p>32)throw ::jxx::lang::IllegalArgumentException("policy");horizontalPolicy_=p;} ::jxx::lang::jint JScrollPane::getHorizontalScrollBarPolicy()const{return horizontalPolicy_;}
    ::jxx::Ptr<JScrollBar> JScrollPane::getVerticalScrollBar()const{return vertical_;} ::jxx::Ptr<JScrollBar> JScrollPane::getHorizontalScrollBar()const{return horizontal_;}
}
