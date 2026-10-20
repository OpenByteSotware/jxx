#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::awt { class Image; }

namespace jxx::awt::image {

class ImageObserver
    : public ::jxx::lang::InterfaceBase<ImageObserver> {
public:
    static constexpr ::jxx::lang::jint WIDTH = 1;
    static constexpr ::jxx::lang::jint HEIGHT = 2;
    static constexpr ::jxx::lang::jint PROPERTIES = 4;
    static constexpr ::jxx::lang::jint SOMEBITS = 8;
    static constexpr ::jxx::lang::jint FRAMEBITS = 16;
    static constexpr ::jxx::lang::jint ALLBITS = 32;
    static constexpr ::jxx::lang::jint ERROR = 64;
    static constexpr ::jxx::lang::jint ABORT = 128;

    ~ImageObserver() override = default;

    virtual ::jxx::lang::jbool imageUpdate(
        const ::jxx::Ptr<::jxx::awt::Image>& image,
        ::jxx::lang::jint infoflags,
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height) = 0;
};

} // namespace jxx::awt::image
