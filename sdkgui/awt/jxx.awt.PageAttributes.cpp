#include "awt/jxx.awt.PageAttributes.h"

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    PageAttributes::PageAttributes() : Super(){}
    PageAttributes::PageAttributes(ColorType c,MediaType m,
        OrientationRequestedType o,OriginType r,PrintQualityType q,
        const ResolutionArray& p):Super(),color_(c),media_(m),orientation_(o),origin_(r),quality_(q){setPrinterResolution(p);}
    PageAttributes::PageAttributes(const ::jxx::Ptr<PageAttributes>& s):Super(){set(s);}
    PageAttributes::ColorType PageAttributes::getColor()const{return color_;} void PageAttributes::setColor(ColorType v){color_=v;}
    PageAttributes::MediaType PageAttributes::getMedia()const{return media_;} void PageAttributes::setMedia(MediaType v){media_=v;}
    PageAttributes::OrientationRequestedType PageAttributes::getOrientationRequested()const{return orientation_;} void PageAttributes::setOrientationRequested(OrientationRequestedType v){orientation_=v;}
    PageAttributes::OriginType PageAttributes::getOrigin()const{return origin_;} void PageAttributes::setOrigin(OriginType v){origin_=v;}
    PageAttributes::PrintQualityType PageAttributes::getPrintQuality()const{return quality_;} void PageAttributes::setPrintQuality(PrintQualityType v){quality_=v;}
    PageAttributes::ResolutionArray PageAttributes::getPrinterResolution()const{auto a=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint,1>>(3);for(::jxx::lang::jint i=0;i<3;++i)(*a)[i]=resolution_[i];return a;}
    void PageAttributes::setPrinterResolution(const ResolutionArray& v){if(!v||v->length!=3||(*v)[0]<=0||(*v)[1]<=0||(*v)[2]<3||(*v)[2]>4)throw ::jxx::lang::IllegalArgumentException("printerResolution");for(::jxx::lang::jint i=0;i<3;++i)resolution_[i]=(*v)[i];}
    void PageAttributes::set(const ::jxx::Ptr<PageAttributes>& s){if(!s)throw ::jxx::lang::IllegalArgumentException("source");color_=s->color_;media_=s->media_;orientation_=s->orientation_;origin_=s->origin_;quality_=s->quality_;resolution_=s->resolution_;}
    ::jxx::Ptr<::jxx::lang::Object> PageAttributes::clone()const{return ::jxx::NEW<PageAttributes>(::jxx::CAST<PageAttributes>(thisPtr()));}
    ::jxx::lang::jbool PageAttributes::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const{auto p=::jxx::CAST<PageAttributes>(o);return p&&color_==p->color_&&media_==p->media_&&orientation_==p->orientation_&&origin_==p->origin_&&quality_==p->quality_&&resolution_==p->resolution_;}
    ::jxx::lang::jint PageAttributes::hashCode()const{return static_cast<::jxx::lang::jint>(color_)^(static_cast<::jxx::lang::jint>(media_)<<2)^(resolution_[0]<<4)^(resolution_[1]<<10);}
    ::jxx::Ptr<::jxx::lang::String> PageAttributes::toString()const{std::ostringstream s;s<<"PageAttributes[resolution="<<resolution_[0]<<"x"<<resolution_[1]<<"]";return ::jxx::NEW<::jxx::lang::String>(s.str());}
}
