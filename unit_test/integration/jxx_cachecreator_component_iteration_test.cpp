#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"

namespace {

class ActionComponent
    : public ::jxx::lang::ClassBase<ActionComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<ActionComponent, JxxSuper>;
};

class HotComponent final
    : public ::jxx::lang::ClassBase<HotComponent, ActionComponent> {
public:
    using JxxSuper = ActionComponent;
    using Super = ::jxx::lang::ClassBase<HotComponent, JxxSuper>;
};

class PlainComponent final
    : public ::jxx::lang::ClassBase<PlainComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<PlainComponent, JxxSuper>;
};

TEST(CacheCreatorSdkIntegration, ComponentSnapshotSupportsInstanceofStyleFiltering)
{
    ::jxx::initialize();
    auto panel = ::jxx::NEW<::jxx::awt::Container>();
    auto action = ::jxx::NEW<ActionComponent>();
    auto hot = ::jxx::NEW<HotComponent>();
    auto plain = ::jxx::NEW<PlainComponent>();
    panel->add(action);
    panel->add(hot);
    panel->add(plain);

    auto components = panel->getComponents();
    ASSERT_NE(components, nullptr);
    ASSERT_EQ(components->size(), 3U);

    ::jxx::lang::jint hidden = 0;
    for (std::size_t index = 0; index < components->size(); ++index)
    {
        const auto& component = (*components)[index];
        if (::jxx::CAST<ActionComponent>(component) != nullptr ||
            ::jxx::CAST<HotComponent>(component) != nullptr)
        {
            component->setVisible(false);
            ++hidden;
        }
    }

    EXPECT_EQ(hidden, 2);
    EXPECT_FALSE(action->isVisible());
    EXPECT_FALSE(hot->isVisible());
    EXPECT_TRUE(plain->isVisible());
}

TEST(CacheCreatorSdkIntegration, ParentEqualityTracksActualContainerIdentity)
{
    ::jxx::initialize();
    auto firstPanel = ::jxx::NEW<::jxx::awt::Container>();
    auto secondPanel = ::jxx::NEW<::jxx::awt::Container>();
    auto button = ::jxx::NEW<HotComponent>();
    firstPanel->add(button);

    ASSERT_NE(button->getParent(), nullptr);
    EXPECT_TRUE(button->getParent()->equals(firstPanel));
    EXPECT_FALSE(button->getParent()->equals(secondPanel));

    secondPanel->add(button);
    EXPECT_EQ(firstPanel->getComponentCount(), 0);
    EXPECT_EQ(secondPanel->getComponentCount(), 1);
    EXPECT_TRUE(button->getParent()->equals(secondPanel));
}

} // namespace
