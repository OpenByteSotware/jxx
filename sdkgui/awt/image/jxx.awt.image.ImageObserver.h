#pragma once
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::awt { class Image; }
namespace jxx::awt::image {
class ImageObserver {
public:
    static constexpr ::jxx::lang::jint WIDTH=1, HEIGHT=2, PROPERTIES=4, SOMEBITS=8, FRAMEBITS=16, ALLBITS=32, ERROR=64, ABORT=128;
    virtual ~ImageObserver()=default;
    virtual ::jxx::lang::jbool imageUpdate(const ::jxx::Ptr<::jxx::awt::Image>& image, ::jxx::lang::jint infoflags, ::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint width, ::jxx::lang::jint height)=0;
}; }
