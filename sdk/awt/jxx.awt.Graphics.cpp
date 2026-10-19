#include "awt/jxx.awt.Graphics.h"

#include "awt/jxx.awt.Image.h"

#include "awt/jxx.awt.Image.h"

#include "awt/jxx.awt.Image.h"

namespace jxx::awt
{
    Graphics::Graphics() : Super(){}
    ::jxx::Ptr<Graphics> Graphics::create(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height) const
    {
        auto result=create();
        if(result){result->translate(x,y);result->clipRect(0,0,width,height);}
        return result;
    }
    ::jxx::Ptr<::jxx::lang::String> Graphics::toString() const
    {return ::jxx::NEW<::jxx::lang::String>("jxx.awt.Graphics");}
    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>&,::jxx::lang::jint,::jxx::lang::jint,const ::jxx::Ptr<::jxx::awt::image::ImageObserver>&){return false;}
}
