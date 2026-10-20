#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
#include "lang/jxx.lang.Math.h"

namespace {

class CacheComponent final
    : public ::jxx::lang::ClassBase<CacheComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<CacheComponent, JxxSuper>;
};

TEST(CacheCreatorSdkIntegration, GeometrySizingAndParentingWorkTogether)
{
    ::jxx::initialize();
    auto panel = ::jxx::NEW<::jxx::awt::Container>();
    panel->setBounds(0, 0, 320, 240);
    auto component = ::jxx::NEW<CacheComponent>();
    component->setSize(80, 40);

    const auto newWidth = static_cast<::jxx::lang::jint>(
        ::jxx::lang::Math::round(
            static_cast<::jxx::lang::jdouble>(panel->getWidth()) / 2.0));
    const auto newHeight = static_cast<::jxx::lang::jint>(
        ::jxx::lang::Math::round(
            static_cast<::jxx::lang::jdouble>(component->getHeight()) /
            (static_cast<::jxx::lang::jdouble>(component->getWidth()) /
             newWidth)));
    component->setSize(newWidth, newHeight);
    component->setLocation(
        (panel->getWidth() - component->getWidth()) / 2,
        (panel->getHeight() - component->getHeight()) / 2);
    panel->add(component);

    auto size = component->getSize();
    auto location = component->getLocation();
    ASSERT_NE(size, nullptr);
    ASSERT_NE(location, nullptr);
    EXPECT_EQ(size->width, 160);
    EXPECT_EQ(size->height, 80);
    EXPECT_EQ(location->x, 80);
    EXPECT_EQ(location->y, 80);
    EXPECT_EQ(component->getParent(), panel);

    auto children = panel->getComponents();
    ASSERT_NE(children, nullptr);
    ASSERT_EQ(children->size(), 1U);
    EXPECT_EQ((*children)(0), component);

    panel->remove(component);
    EXPECT_EQ(component->getParent(), nullptr);
    EXPECT_EQ(panel->getComponentCount(), 0);
}

TEST(CacheCreatorSdkIntegration, DirectionAndRgbaColorValuesMatchSourceOperations)
{
    ::jxx::initialize();
    const auto direction = ::jxx::lang::Math::toRadians(7.0 * 45.0);
    EXPECT_NEAR(direction, 7.0 * ::jxx::lang::Math::PI / 4.0, 1.0e-12);

    auto color = ::jxx::NEW<::jxx::awt::Color>(10, 20, 30, 40);
    EXPECT_EQ(color->getRed(), 10);
    EXPECT_EQ(color->getGreen(), 20);
    EXPECT_EQ(color->getBlue(), 30);
    EXPECT_EQ(color->getAlpha(), 40);
    EXPECT_EQ(static_cast<std::uint32_t>(color->getRGB()), 0x280A141EU);
}

TEST(CacheCreatorSdkIntegration, IntegerGeometryUsesExpectedAbsoluteOffsets)
{
    ::jxx::initialize();
    const ::jxx::lang::jint pressedOffsetX = -4;
    const ::jxx::lang::jint pressedOffsetY = 3;
    EXPECT_EQ(::jxx::lang::Math::abs(pressedOffsetX), 4);
    EXPECT_EQ(::jxx::lang::Math::abs(pressedOffsetY), 3);
}

} // namespace
