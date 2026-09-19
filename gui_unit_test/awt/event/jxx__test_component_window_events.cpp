#include <gtest/gtest.h>

#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.WindowEvent.h"

TEST(ComponentEventTest, ConstructorPreservesIdentifierAndSource)
{
    jxx::awt::event::ComponentEvent event(
        nullptr,
        jxx::awt::event::ComponentEvent::COMPONENT_SHOWN);

    EXPECT_EQ(event.getID(), jxx::awt::event::ComponentEvent::COMPONENT_SHOWN);
    EXPECT_EQ(event.getComponent(), nullptr);
}

TEST(FocusEventTest, ConstructorPreservesTemporaryAndOppositeComponent)
{
    jxx::awt::event::FocusEvent event(
        nullptr,
        jxx::awt::event::FocusEvent::FOCUS_LOST,
        true,
        nullptr);

    EXPECT_EQ(event.getID(), jxx::awt::event::FocusEvent::FOCUS_LOST);
    EXPECT_TRUE(event.isTemporary());
    EXPECT_EQ(event.getOppositeComponent(), nullptr);
}

TEST(WindowEventTest, StateChangeConstructorPreservesOldAndNewState)
{
    jxx::awt::event::WindowEvent event(
        nullptr,
        jxx::awt::event::WindowEvent::WINDOW_STATE_CHANGED,
        1,
        6);

    EXPECT_EQ(event.getID(), jxx::awt::event::WindowEvent::WINDOW_STATE_CHANGED);
    EXPECT_EQ(event.getWindow(), nullptr);
    EXPECT_EQ(event.getOldState(), 1);
    EXPECT_EQ(event.getNewState(), 6);
}
