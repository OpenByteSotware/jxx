#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net { class URL; }
namespace jxx::awt
{
    class Dimension;
    class Graphics2D;
    class Rectangle;

    class SplashScreen final : public ::jxx::lang::ClassBase<
        SplashScreen, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<SplashScreen, JxxSuper>;

        static ::jxx::Ptr<SplashScreen> getSplashScreen();
        ::jxx::lang::jbool isVisible() const;
        ::jxx::Ptr<Rectangle> getBounds() const;
        ::jxx::Ptr<Dimension> getSize() const;
        ::jxx::Ptr<::jxx::net::URL> getImageURL() const;
        void setImageURL(const ::jxx::Ptr<::jxx::net::URL>& imageURL);
        ::jxx::Ptr<Graphics2D> createGraphics();
        void update();
        void close();

    private:
        SplashScreen();
        ::jxx::lang::jbool visible_ = false;
        ::jxx::Ptr<::jxx::net::URL> imageURL_;
    };
}
