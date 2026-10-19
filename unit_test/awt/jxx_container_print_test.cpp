#include <gtest/gtest.h>

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Graphics.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"

namespace {

class FilledComponent final : public ::jxx::awt::Component {
public:
    explicit FilledComponent(const ::jxx::Ptr<::jxx::awt::Color>& color)
        : ::jxx::awt::Component(), color_(color) {}

    void paint(const ::jxx::Ptr<::jxx::awt::Graphics>& graphics) override
    {
        if (graphics == nullptr) return;
        graphics->setColor(color_);
        graphics->fillRect(-10, -10, getWidth() + 20, getHeight() + 20);
    }

private:
    ::jxx::Ptr<::jxx::awt::Color> color_;
};

TEST(ContainerPrintParity, PrintAllRendersVisibleChildrenAtTheirBounds)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto root = ::jxx::NEW<::jxx::awt::Container>();
    root->setBounds(0, 0, 8, 8);
    auto red = ::jxx::NEW<::jxx::awt::Color>(255, 0, 0);
    auto child = ::jxx::NEW<FilledComponent>(red);
    child->setBounds(2, 3, 3, 2);
    root->add(child);

    auto graphics = image->getGraphics();
    ASSERT_NE(graphics, nullptr);
    root->printAll(graphics);
    graphics->dispose();

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 3)),
        static_cast<std::uint32_t>(red->getRGB()));
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(4, 4)),
        static_cast<std::uint32_t>(red->getRGB()));
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(1, 3)), 0U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(5, 4)), 0U);
}

TEST(ContainerPrintParity, PrintAllSkipsInvisibleChildren)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        4, 4, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto root = ::jxx::NEW<::jxx::awt::Container>();
    root->setBounds(0, 0, 4, 4);
    auto blue = ::jxx::NEW<::jxx::awt::Color>(0, 0, 255);
    auto child = ::jxx::NEW<FilledComponent>(blue);
    child->setBounds(0, 0, 4, 4);
    child->setVisible(false);
    root->add(child);

    auto graphics = image->getGraphics();
    ASSERT_NE(graphics, nullptr);
    root->printAll(graphics);
    graphics->dispose();

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(1, 1)), 0U);
}

TEST(ContainerPrintParity, NestedContainersAccumulateTranslationAndClip)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        10, 10, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto root = ::jxx::NEW<::jxx::awt::Container>();
    root->setBounds(0, 0, 10, 10);
    auto nested = ::jxx::NEW<::jxx::awt::Container>();
    nested->setBounds(2, 2, 4, 4);
    auto green = ::jxx::NEW<::jxx::awt::Color>(0, 255, 0);
    auto child = ::jxx::NEW<FilledComponent>(green);
    child->setBounds(1, 1, 2, 2);
    nested->add(child);
    root->add(nested);

    auto graphics = image->getGraphics();
    ASSERT_NE(graphics, nullptr);
    root->printAll(graphics);
    graphics->dispose();

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(3, 3)),
        static_cast<std::uint32_t>(green->getRGB()));
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(4, 4)),
        static_cast<std::uint32_t>(green->getRGB()));
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 3)), 0U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(5, 4)), 0U);
}

} // namespace
