#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing
{
    class BoundedRangeModel : public ::jxx::lang::InterfaceBase<BoundedRangeModel>
    {
    public:~BoundedRangeModel() override=default;virtual ::jxx::lang::jint getMinimum()const=0;virtual void setMinimum(::jxx::lang::jint value)=0;virtual ::jxx::lang::jint getMaximum()const=0;virtual void setMaximum(::jxx::lang::jint value)=0;virtual ::jxx::lang::jint getValue()const=0;virtual void setValue(::jxx::lang::jint value)=0;virtual ::jxx::lang::jint getExtent()const=0;virtual void setExtent(::jxx::lang::jint value)=0;virtual ::jxx::lang::jbool getValueIsAdjusting()const=0;virtual void setValueIsAdjusting(::jxx::lang::jbool value)=0;
    };
}
