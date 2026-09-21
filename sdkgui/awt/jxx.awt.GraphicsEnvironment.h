#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt::image
{
    class BufferedImage;
}

namespace jxx::util
{
    class Locale;
}

namespace jxx::awt
{
    class Font;
    class Graphics2D;
    class GraphicsDevice;
    class Point;
    class Rectangle;

    class GraphicsEnvironment : public ::jxx::lang::ClassBase<
        GraphicsEnvironment, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GraphicsEnvironment, JxxSuper>;
        using DeviceArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<GraphicsDevice>, 1>>;
        using FontArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<Font>, 1>>;
        using StringArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::String>, 1>>;

        ~GraphicsEnvironment() override = default;

        static ::jxx::Ptr<GraphicsEnvironment>
            getLocalGraphicsEnvironment();
        static ::jxx::lang::jbool isHeadless();

        virtual DeviceArray getScreenDevices() const = 0;
        virtual ::jxx::Ptr<GraphicsDevice>
            getDefaultScreenDevice() const = 0;
        virtual ::jxx::Ptr<Graphics2D> createGraphics(
            const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image) = 0;
        virtual FontArray getAllFonts() const = 0;
        virtual StringArray getAvailableFontFamilyNames() const = 0;
        virtual StringArray getAvailableFontFamilyNames(
            const ::jxx::Ptr<::jxx::util::Locale>& locale) const = 0;
        virtual ::jxx::lang::jbool registerFont(
            const ::jxx::Ptr<Font>& font);
        virtual void preferLocaleFonts();
        virtual void preferProportionalFonts();
        virtual ::jxx::Ptr<Point> getCenterPoint() const;
        virtual ::jxx::Ptr<Rectangle> getMaximumWindowBounds() const;
        virtual ::jxx::lang::jbool isHeadlessInstance() const;

    protected:
        GraphicsEnvironment();
    };
}
