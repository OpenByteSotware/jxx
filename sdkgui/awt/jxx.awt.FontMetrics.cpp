#include "awt/jxx.awt.FontMetrics.h"

#include <sstream>
#include "awt/jxx.awt.Font.h"

namespace jxx::awt
{
    FontMetrics::FontMetrics(const ::jxx::Ptr<Font>& font):Super(),font_(font){}
    ::jxx::Ptr<Font> FontMetrics::getFont()const{return font_;}
    ::jxx::lang::jint FontMetrics::getLeading()const{return 0;}
    ::jxx::lang::jint FontMetrics::getAscent()const{return font_?font_->getSize():0;}
    ::jxx::lang::jint FontMetrics::getDescent()const{return font_?font_->getSize()/4:0;}
    ::jxx::lang::jint FontMetrics::getHeight()const{return getLeading()+getAscent()+getDescent();}
    ::jxx::lang::jint FontMetrics::getMaxAscent()const{return getAscent();}
    ::jxx::lang::jint FontMetrics::getMaxDescent()const{return getDescent();}
    ::jxx::lang::jint FontMetrics::charWidth(::jxx::lang::jchar)const{return font_?font_->getSize()/2:0;}
    ::jxx::lang::jint FontMetrics::stringWidth(const ::jxx::Ptr<::jxx::lang::String>& text)const{return text?static_cast<::jxx::lang::jint>(text->length())*charWidth(0):0;}
    FontMetrics::WidthArray FontMetrics::getWidths()const{auto result=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint,1>>(256);for(::jxx::lang::jint i=0;i<256;++i)(*result)[i]=charWidth(static_cast<::jxx::lang::jchar>(i));return result;}
    ::jxx::lang::jbool FontMetrics::hasUniformLineMetrics()const{return true;}
    ::jxx::Ptr<::jxx::lang::String> FontMetrics::toString()const{std::ostringstream out;out<<"jxx.awt.FontMetrics[ascent="<<getAscent()<<",descent="<<getDescent()<<"]";return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
