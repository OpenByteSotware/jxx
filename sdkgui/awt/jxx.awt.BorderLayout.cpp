#include "awt/jxx.awt.BorderLayout.h"

#include <algorithm>
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace { ::jxx::Ptr<::jxx::lang::String> text(const char* value) { return ::jxx::NEW<::jxx::lang::String>(value); } }
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::NORTH=text("North"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::SOUTH=text("South"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::EAST=text("East"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::WEST=text("West"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::CENTER=text("Center");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::BEFORE_FIRST_LINE=text("First"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::AFTER_LAST_LINE=text("Last"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::BEFORE_LINE_BEGINS=text("Before"); ::jxx::Ptr<::jxx::lang::String> BorderLayout::AFTER_LINE_ENDS=text("After");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::PAGE_START=BorderLayout::BEFORE_FIRST_LINE; ::jxx::Ptr<::jxx::lang::String> BorderLayout::PAGE_END=BorderLayout::AFTER_LAST_LINE; ::jxx::Ptr<::jxx::lang::String> BorderLayout::LINE_START=BorderLayout::BEFORE_LINE_BEGINS; ::jxx::Ptr<::jxx::lang::String> BorderLayout::LINE_END=BorderLayout::AFTER_LINE_ENDS;

    BorderLayout::BorderLayout() : BorderLayout(0, 0) {}
    BorderLayout::BorderLayout(::jxx::lang::jint hgap, ::jxx::lang::jint vgap) : hgap_(hgap), vgap_(vgap) {}
    ::jxx::lang::jint BorderLayout::getHgap() const { return hgap_; }
    void BorderLayout::setHgap(::jxx::lang::jint hgap) { hgap_ = hgap; }
    ::jxx::lang::jint BorderLayout::getVgap() const { return vgap_; }
    void BorderLayout::setVgap(::jxx::lang::jint vgap) { vgap_ = vgap; }

    void BorderLayout::addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<Component>& component)
    {
        const std::string key = name ? name->utf8() : "Center";
        if (key!="North"&&key!="South"&&key!="East"&&key!="West"&&key!="Center"&&key!="First"&&key!="Last"&&key!="Before"&&key!="After")
            throw ::jxx::lang::IllegalArgumentException("unknown border constraint");
        components_[key] = component;
    }
    void BorderLayout::removeLayoutComponent(const ::jxx::Ptr<Component>& component)
    {
        for (auto it=components_.begin(); it!=components_.end(); ) { if (it->second==component) it=components_.erase(it); else ++it; }
    }
    ::jxx::Ptr<Component> BorderLayout::componentFor(const char* key) const { auto it=components_.find(key); return it==components_.end()?nullptr:it->second; }
    ::jxx::Ptr<Dimension> BorderLayout::preferredLayoutSize(const ::jxx::Ptr<Container>&)
    {
        auto n=componentFor("First"); if(!n)n=componentFor("North"); auto s=componentFor("Last"); if(!s)s=componentFor("South"); auto w=componentFor("Before"); if(!w)w=componentFor("West"); auto e=componentFor("After"); if(!e)e=componentFor("East"); auto c=componentFor("Center");
        const auto middleHeight=std::max({w?w->getHeight():0,e?e->getHeight():0,c?c->getHeight():0});
        const auto middleWidth=(w?w->getWidth()+hgap_:0)+(c?c->getWidth():0)+(e?hgap_+e->getWidth():0);
        const auto width=std::max({middleWidth,n?n->getWidth():0,s?s->getWidth():0});
        const auto height=(n?n->getHeight()+vgap_:0)+middleHeight+(s?vgap_+s->getHeight():0);
        return ::jxx::NEW<Dimension>(width,height);
    }
    ::jxx::Ptr<Dimension> BorderLayout::minimumLayoutSize(const ::jxx::Ptr<Container>& parent) { return preferredLayoutSize(parent); }
    void BorderLayout::layoutContainer(const ::jxx::Ptr<Container>& parent)
    {
        if(!parent)return; auto top=0,left=0,right=parent->getWidth(),bottom=parent->getHeight();
        auto n=componentFor("First");if(!n)n=componentFor("North"); auto s=componentFor("Last");if(!s)s=componentFor("South"); auto w=componentFor("Before");if(!w)w=componentFor("West"); auto e=componentFor("After");if(!e)e=componentFor("East"); auto c=componentFor("Center");
        if(n&&n->isVisible()){n->setBounds(left,top,right-left,n->getHeight());top+=n->getHeight()+vgap_;}
        if(s&&s->isVisible()){bottom-=s->getHeight();s->setBounds(left,bottom,right-left,s->getHeight());bottom-=vgap_;}
        if(e&&e->isVisible()){right-=e->getWidth();e->setBounds(right,top,e->getWidth(),bottom-top);right-=hgap_;}
        if(w&&w->isVisible()){w->setBounds(left,top,w->getWidth(),bottom-top);left+=w->getWidth()+hgap_;}
        if(c&&c->isVisible())c->setBounds(left,top,right-left,bottom-top);
    }
}
