#include <gtest/gtest.h>

#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "awt/event/jxx.awt.event.MouseWheelEvent.h"
#include "awt/event/jxx.awt.event.TextEvent.h"

TEST(TextEventTest, IdentifierMatchesTextValueChanged)
{
    jxx::awt::event::TextEvent event(
        nullptr, jxx::awt::event::TextEvent::TEXT_VALUE_CHANGED);
    EXPECT_EQ(event.getID(), jxx::awt::event::TextEvent::TEXT_VALUE_CHANGED);
}

TEST(HierarchyEventTest, ConstructorPreservesChangedObjectsAndFlags)
{
    jxx::awt::event::HierarchyEvent event(
        nullptr,
        jxx::awt::event::HierarchyEvent::HIERARCHY_CHANGED,
        nullptr,
        nullptr,
        jxx::awt::event::HierarchyEvent::PARENT_CHANGED);
    EXPECT_EQ(event.getID(), jxx::awt::event::HierarchyEvent::HIERARCHY_CHANGED);
    EXPECT_EQ(event.getChanged(), nullptr);
    EXPECT_EQ(event.getChangedParent(), nullptr);
    EXPECT_EQ(event.getChangeFlags(), jxx::awt::event::HierarchyEvent::PARENT_CHANGED);
}

TEST(MouseWheelEventTest, ScrollPropertiesRoundTrip)
{
    jxx::awt::event::MouseWheelEvent event(
        nullptr,
        jxx::awt::event::MouseEvent::MOUSE_WHEEL,
        25,
        0,
        4,
        6,
        1,
        false,
        jxx::awt::event::MouseWheelEvent::WHEEL_UNIT_SCROLL,
        3,
        -2,
        -2.0);
    EXPECT_EQ(event.getScrollType(), jxx::awt::event::MouseWheelEvent::WHEEL_UNIT_SCROLL);
    EXPECT_EQ(event.getScrollAmount(), 3);
    EXPECT_EQ(event.getWheelRotation(), -2);
    EXPECT_DOUBLE_EQ(event.getPreciseWheelRotation(), -2.0);
    EXPECT_EQ(event.getUnitsToScroll(), -6);
}
