#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"

namespace {

class FirstComponent final
    : public ::jxx::lang::ClassBase<FirstComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<FirstComponent, JxxSuper>;
};

class SecondComponent final
    : public ::jxx::lang::ClassBase<SecondComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<SecondComponent, JxxSuper>;
};

TEST(ContainerParity, GetComponentsReturnsChildrenInZOrder)
{
    ::jxx::initialize();
    auto container = ::jxx::NEW<::jxx::awt::Container>();
    auto first = ::jxx::NEW<FirstComponent>();
    auto second = ::jxx::NEW<SecondComponent>();
    container->add(first);
    container->add(second);

    auto children = container->getComponents();
    ASSERT_NE(children, nullptr);
    ASSERT_EQ(children->size(), 2);
    EXPECT_EQ((*children)(0), first);
    EXPECT_EQ((*children)(1), second);
    EXPECT_NE(::jxx::CAST<FirstComponent>((*children)(0)), nullptr);
    EXPECT_NE(::jxx::CAST<SecondComponent>((*children)(1)), nullptr);
}

TEST(ContainerParity, GetComponentsReturnsIndependentSnapshot)
{
    ::jxx::initialize();
    auto container = ::jxx::NEW<::jxx::awt::Container>();
    auto first = ::jxx::NEW<FirstComponent>();
    container->add(first);

    auto snapshot = container->getComponents();
    ASSERT_EQ(snapshot->size(), 1);
    (*snapshot)(0) = nullptr;
    EXPECT_EQ(container->getComponentCount(), 1);
    EXPECT_EQ(container->getComponent(0), first);

    container->add(::jxx::NEW<SecondComponent>());
    EXPECT_EQ(snapshot->size(), 1);
    EXPECT_EQ(container->getComponents()->size(), 2);
}

TEST(ContainerParity, GetComponentsReturnsEmptyArrayForEmptyContainer)
{
    ::jxx::initialize();
    auto container = ::jxx::NEW<::jxx::awt::Container>();
    auto children = container->getComponents();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 0);
}

} // namespace
