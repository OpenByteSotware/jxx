#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.MenuContainer.h"
#include "awt/image/jxx.awt.image.ImageObserver.h"
#include "io/jxx.io.SerializableI.h"

namespace {
class HierarchyComponent final
    : public ::jxx::lang::ClassBase<HierarchyComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<HierarchyComponent, JxxSuper>;
};

TEST(ComponentHierarchyParity, ImplementsRequiredInterfaces)
{
    ::jxx::initialize();
    auto component = ::jxx::NEW<HierarchyComponent>();
    EXPECT_NE(::jxx::CAST<::jxx::awt::image::ImageObserver>(component), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::awt::MenuContainer>(component), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::io::SerializableI>(component), nullptr);
}

TEST(ComponentHierarchyParity, ImageObserverTerminalFlagsStopUpdates)
{
    ::jxx::initialize();
    auto component = ::jxx::NEW<HierarchyComponent>();
    EXPECT_TRUE(component->imageUpdate(nullptr,
        ::jxx::awt::image::ImageObserver::SOMEBITS, 0, 0, 1, 1));
    EXPECT_FALSE(component->imageUpdate(nullptr,
        ::jxx::awt::image::ImageObserver::ALLBITS, 0, 0, 1, 1));
    EXPECT_FALSE(component->imageUpdate(nullptr,
        ::jxx::awt::image::ImageObserver::ERROR, 0, 0, 1, 1));
}

TEST(ComponentHierarchyParity, InterfaceMetadataIsInitialized)
{
    ::jxx::initialize();
    EXPECT_NE(::jxx::awt::Component::Class(), nullptr);
    EXPECT_NE(::jxx::awt::MenuContainer::Class(), nullptr);
    EXPECT_NE(::jxx::awt::image::ImageObserver::Class(), nullptr);
}
} // namespace
