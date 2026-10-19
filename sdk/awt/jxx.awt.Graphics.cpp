#include "awt/jxx.awt.Graphics.h"

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
    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>&,
        ::jxx::lang::jint, ::jxx::lang::jint,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>&) { return false; }

    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>& image,
        ::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint, ::jxx::lang::jint,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>& observer)
    { return drawImage(image, x, y, observer); }

    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>& image,
        ::jxx::lang::jint x, ::jxx::lang::jint y,
        const ::jxx::Ptr<Color>&,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>& observer)
    { return drawImage(image, x, y, observer); }

    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>& image,
        ::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint width, ::jxx::lang::jint height,
        const ::jxx::Ptr<Color>&,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>& observer)
    { return drawImage(image, x, y, width, height, observer); }

    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>&,
        ::jxx::lang::jint, ::jxx::lang::jint, ::jxx::lang::jint,
        ::jxx::lang::jint, ::jxx::lang::jint, ::jxx::lang::jint,
        ::jxx::lang::jint, ::jxx::lang::jint,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>&) { return false; }

    ::jxx::lang::jbool Graphics::drawImage(const ::jxx::Ptr<Image>& image,
        ::jxx::lang::jint dx1, ::jxx::lang::jint dy1,
        ::jxx::lang::jint dx2, ::jxx::lang::jint dy2,
        ::jxx::lang::jint sx1, ::jxx::lang::jint sy1,
        ::jxx::lang::jint sx2, ::jxx::lang::jint sy2,
        const ::jxx::Ptr<Color>&,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>& observer)
    { return drawImage(image, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, observer); }
}
