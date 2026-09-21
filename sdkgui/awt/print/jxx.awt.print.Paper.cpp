#include "awt/print/jxx.awt.print.Paper.h"

namespace jxx::awt::print
{
    Paper::Paper() : Super(){}
    ::jxx::Ptr<::jxx::lang::Object> Paper::clone() const
    {
        auto result = ::jxx::NEW<Paper>();
        result->setSize(width_, height_);
        result->setImageableArea(imageableX_, imageableY_,
            imageableWidth_, imageableHeight_);
        return result;
    }
    ::jxx::lang::jdouble Paper::getHeight() const{return height_;}
    ::jxx::lang::jdouble Paper::getWidth() const{return width_;}
    void Paper::setSize(::jxx::lang::jdouble w,::jxx::lang::jdouble h){width_=w;height_=h;}
    void Paper::setImageableArea(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h){imageableX_=x;imageableY_=y;imageableWidth_=w;imageableHeight_=h;}
    ::jxx::lang::jdouble Paper::getImageableX() const{return imageableX_;}
    ::jxx::lang::jdouble Paper::getImageableY() const{return imageableY_;}
    ::jxx::lang::jdouble Paper::getImageableWidth() const{return imageableWidth_;}
    ::jxx::lang::jdouble Paper::getImageableHeight() const{return imageableHeight_;}
}
