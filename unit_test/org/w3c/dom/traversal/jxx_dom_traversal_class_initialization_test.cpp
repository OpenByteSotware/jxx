#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.ClassInitialization.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"

namespace {

auto nodeFilterClass()
{
    return ::jxx::lang::InterfaceBase<
        ::jxx::org::w3c::dom::traversal::NodeFilter>::Class();
}

} // namespace

TEST(JxxDomTraversalClassInitialization, NodeFilterHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(nodeFilterClass(), nullptr);
}

TEST(JxxDomTraversalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = nodeFilterClass();

    EXPECT_NO_THROW(
        ::jxx::org::w3c::dom::traversal::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::org::w3c::dom::traversal::initializeClasses());

    EXPECT_EQ(before, nodeFilterClass());
}
