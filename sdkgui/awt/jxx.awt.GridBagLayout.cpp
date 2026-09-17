#include "awt/jxx.awt.GridBagLayout.h"
#include <algorithm>
#include <limits>
#include <sstream>
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt {
GridBagLayout::GridBagLayout():defaults_(::jxx::NEW<GridBagConstraints>()){}
void GridBagLayout::setConstraints(const ::jxx::Ptr<Component>& c,const ::jxx::Ptr<GridBagConstraints>& v){if(!c||!v)throw ::jxx::lang::IllegalArgumentException("constraints");constraints_[c.get()]=::jxx::CAST<GridBagConstraints>(v->clone());}
::jxx::Ptr<GridBagConstraints> GridBagLayout::lookupConstraints(const ::jxx::Ptr<Component>& c) const{auto i=constraints_.find(c.get());return i==constraints_.end()?defaults_:i->second;}
::jxx::Ptr<GridBagConstraints> GridBagLayout::getConstraints(const ::jxx::Ptr<Component>& c) const{return ::jxx::CAST<GridBagConstraints>(lookupConstraints(c)->clone());}
void GridBagLayout::addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<Component>& c){if(c)constraints_[c.get()]=::jxx::CAST<GridBagConstraints>(defaults_->clone());}
void GridBagLayout::addLayoutComponent(const ::jxx::Ptr<Component>& c,const ::jxx::Ptr<::jxx::lang::Object>& v){if(!c)return;if(!v){constraints_[c.get()]=::jxx::CAST<GridBagConstraints>(defaults_->clone());return;}auto g=::jxx::CAST<GridBagConstraints>(v);if(!g)throw ::jxx::lang::IllegalArgumentException("constraints");setConstraints(c,g);}
void GridBagLayout::removeLayoutComponent(const ::jxx::Ptr<Component>& c){if(c)constraints_.erase(c.get());}
::jxx::Ptr<Dimension> GridBagLayout::preferredLayoutSize(const ::jxx::Ptr<Container>& p){::jxx::lang::jint maxX=0,maxY=0,cursor=0;if(p)for(::jxx::lang::jint i=0;i<p->getComponentCount();++i){auto c=p->getComponent(i);auto g=lookupConstraints(c);auto x=g->gridx<0?cursor:g->gridx;auto y=g->gridy<0?0:g->gridy;auto w=std::max(1,g->gridwidth);auto h=std::max(1,g->gridheight);maxX=std::max(maxX,x+w);maxY=std::max(maxY,y+h);cursor=x+w;}return ::jxx::NEW<Dimension>(maxX*100,maxY*30);}
::jxx::Ptr<Dimension> GridBagLayout::minimumLayoutSize(const ::jxx::Ptr<Container>& p){return preferredLayoutSize(p);}
::jxx::Ptr<Dimension> GridBagLayout::maximumLayoutSize(const ::jxx::Ptr<Container>&){auto m=std::numeric_limits<::jxx::lang::jint>::max();return ::jxx::NEW<Dimension>(m,m);}
::jxx::lang::jfloat GridBagLayout::getLayoutAlignmentX(const ::jxx::Ptr<Container>&){return 0.5f;} ::jxx::lang::jfloat GridBagLayout::getLayoutAlignmentY(const ::jxx::Ptr<Container>&){return 0.5f;} void GridBagLayout::invalidateLayout(const ::jxx::Ptr<Container>&){}
void GridBagLayout::layoutContainer(const ::jxx::Ptr<Container>& p){if(!p)return;columns_=rows_=0;::jxx::lang::jint cursor=0;for(::jxx::lang::jint i=0;i<p->getComponentCount();++i){auto c=p->getComponent(i);auto g=lookupConstraints(c);auto x=g->gridx<0?cursor:g->gridx;auto y=g->gridy<0?0:g->gridy;auto w=std::max(1,g->gridwidth);auto h=std::max(1,g->gridheight);columns_=std::max(columns_,x+w);rows_=std::max(rows_,y+h);cursor=x+w;}cellWidth_=columns_?p->getWidth()/columns_:0;cellHeight_=rows_?p->getHeight()/rows_:0;cursor=0;for(::jxx::lang::jint i=0;i<p->getComponentCount();++i){auto c=p->getComponent(i);auto g=lookupConstraints(c);auto x=g->gridx<0?cursor:g->gridx;auto y=g->gridy<0?0:g->gridy;auto w=std::max(1,g->gridwidth);auto h=std::max(1,g->gridheight);auto in=g->insets;auto l=in?in->left:0,t=in?in->top:0,r=in?in->right:0,b=in?in->bottom:0;auto bx=x*cellWidth_+l,by=y*cellHeight_+t,bw=w*cellWidth_-l-r,bh=h*cellHeight_-t-b;auto cw=(g->fill==GridBagConstraints::HORIZONTAL||g->fill==GridBagConstraints::BOTH)?bw:std::min(bw,c->getWidth()+g->ipadx);auto ch=(g->fill==GridBagConstraints::VERTICAL||g->fill==GridBagConstraints::BOTH)?bh:std::min(bh,c->getHeight()+g->ipady);c->setBounds(bx+std::max(0,(bw-cw)/2),by+std::max(0,(bh-ch)/2),std::max(0,cw),std::max(0,ch));cursor=x+w;}}
::jxx::Ptr<Point> GridBagLayout::getLayoutOrigin() const{return ::jxx::NEW<Point>(originX_,originY_);} ::jxx::Ptr<Dimension> GridBagLayout::getLayoutDimensions() const{return ::jxx::NEW<Dimension>(columns_,rows_);} ::jxx::Ptr<Point> GridBagLayout::location(::jxx::lang::jint x,::jxx::lang::jint y) const{return ::jxx::NEW<Point>(cellWidth_?std::max(0,(x-originX_)/cellWidth_):0,cellHeight_?std::max(0,(y-originY_)/cellHeight_):0);} ::jxx::Ptr<::jxx::lang::String> GridBagLayout::toString() const{std::ostringstream o;o<<"jxx.awt.GridBagLayout[columns="<<columns_<<",rows="<<rows_<<"]";return ::jxx::NEW<::jxx::lang::String>(o.str());}
}
