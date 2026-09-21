#include "awt/jxx.awt.GraphicsEnvironment.h"

#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"

namespace jxx::awt
{
    namespace
    {
        class LocalGraphicsEnvironment final : public GraphicsEnvironment
        {
        public:
            DeviceArray getScreenDevices() const override
            {
                return ::jxx::NEW<::jxx::lang::JxxArray<
                    ::jxx::Ptr<GraphicsDevice>, 1>>(0);
            }
            ::jxx::Ptr<GraphicsDevice> getDefaultScreenDevice() const override
            { return nullptr; }
            ::jxx::Ptr<Graphics2D> createGraphics(
                const ::jxx::Ptr<::jxx::awt::image::BufferedImage>&) override
            { return nullptr; }
            FontArray getAllFonts() const override
            {
                return ::jxx::NEW<::jxx::lang::JxxArray<
                    ::jxx::Ptr<Font>, 1>>(0);
            }
            StringArray getAvailableFontFamilyNames() const override
            {
                return ::jxx::NEW<::jxx::lang::JxxArray<
                    ::jxx::Ptr<::jxx::lang::String>, 1>>(0);
            }
            StringArray getAvailableFontFamilyNames(
                const ::jxx::Ptr<::jxx::util::Locale>&) const override
            { return getAvailableFontFamilyNames(); }
        };
    }

    GraphicsEnvironment::GraphicsEnvironment() : Super()
    {
    }

    ::jxx::Ptr<GraphicsEnvironment>
    GraphicsEnvironment::getLocalGraphicsEnvironment()
    {
        static const auto environment =
            ::jxx::NEW<LocalGraphicsEnvironment>();
        return environment;
    }

    ::jxx::lang::jbool GraphicsEnvironment::isHeadless()
    {
        return getLocalGraphicsEnvironment()->isHeadlessInstance();
    }

    ::jxx::lang::jbool GraphicsEnvironment::registerFont(
        const ::jxx::Ptr<Font>& font)
    {
        return font != nullptr;
    }

    void GraphicsEnvironment::preferLocaleFonts()
    {
    }

    void GraphicsEnvironment::preferProportionalFonts()
    {
    }

    ::jxx::Ptr<Point> GraphicsEnvironment::getCenterPoint() const
    {
        const auto bounds = getMaximumWindowBounds();
        return bounds == nullptr ? ::jxx::NEW<Point>() :
            ::jxx::NEW<Point>(bounds->x + bounds->width / 2,
                bounds->y + bounds->height / 2);
    }

    ::jxx::Ptr<Rectangle>
    GraphicsEnvironment::getMaximumWindowBounds() const
    {
        return ::jxx::NEW<Rectangle>();
    }

    ::jxx::lang::jbool GraphicsEnvironment::isHeadlessInstance() const
    {
        return getDefaultScreenDevice() == nullptr;
    }
}
