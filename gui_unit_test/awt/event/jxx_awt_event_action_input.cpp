#include <gtest/gtest.h>

#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.MouseEvent.h"
#include "lang/jxx.lang.String.h"

TEST(ActionEventTest, ConstructorPreservesCommandTimestampAndModifiers)
{
    auto command = jxx::NEW<jxx::lang::String>("save");
    jxx::awt::event::ActionEvent event(
        nullptr,
        jxx::awt::event::ActionEvent::ACTION_PERFORMED,
        command,
        123456,
        jxx::awt::event::ActionEvent::CTRL_MASK);

    EXPECT_EQ(event.getID(), jxx::awt::event::ActionEvent::ACTION_PERFORMED);
    EXPECT_EQ(event.getActionCommand(), command);
    EXPECT_EQ(event.getWhen(), 123456);
    EXPECT_EQ(event.getModifiers(), jxx::awt::event::ActionEvent::CTRL_MASK);
}

TEST(KeyEventTest, AccessorsAndMutatorsPreserveKeyData)
{
    jxx::awt::event::KeyEvent event(
        nullptr,
        jxx::awt::event::KeyEvent::KEY_PRESSED,
        42,
        jxx::awt::event::InputEvent::SHIFT_DOWN_MASK,
        65,
        static_cast<jxx::lang::jchar>('A'));

    EXPECT_EQ(event.getWhen(), 42);
    EXPECT_TRUE(event.isShiftDown());
    EXPECT_FALSE(event.isControlDown());
    EXPECT_EQ(event.getKeyCode(), 65);
    EXPECT_EQ(event.getKeyChar(), static_cast<jxx::lang::jchar>('A'));

    event.setKeyCode(66);
    event.setKeyChar(static_cast<jxx::lang::jchar>('B'));
    EXPECT_EQ(event.getKeyCode(), 66);
    EXPECT_EQ(event.getKeyChar(), static_cast<jxx::lang::jchar>('B'));
}

TEST(MouseEventTest, ConstructorAndPointExposePointerLocation)
{
    jxx::awt::event::MouseEvent event(
        nullptr,
        jxx::awt::event::MouseEvent::MOUSE_PRESSED,
        99,
        jxx::awt::event::InputEvent::BUTTON1_DOWN_MASK,
        17,
        23,
        2,
        true,
        jxx::awt::event::MouseEvent::BUTTON1);

    EXPECT_EQ(event.getX(), 17);
    EXPECT_EQ(event.getY(), 23);
    EXPECT_EQ(event.getClickCount(), 2);
    EXPECT_EQ(event.getButton(), jxx::awt::event::MouseEvent::BUTTON1);
    EXPECT_TRUE(event.isPopupTrigger());

    const auto point = event.getPoint();
    ASSERT_NE(point, nullptr);
    EXPECT_EQ(point->x, 17);
    EXPECT_EQ(point->y, 23);
}

TEST(InputEventTest, ButtonMasksAndConsumedStateFollowContract)
{
    EXPECT_EQ(jxx::awt::event::InputEvent::getMaskForButton(1),
        jxx::awt::event::InputEvent::BUTTON1_DOWN_MASK);
    EXPECT_EQ(jxx::awt::event::InputEvent::getMaskForButton(2),
        jxx::awt::event::InputEvent::BUTTON2_DOWN_MASK);
    EXPECT_EQ(jxx::awt::event::InputEvent::getMaskForButton(3),
        jxx::awt::event::InputEvent::BUTTON3_DOWN_MASK);
    EXPECT_EQ(jxx::awt::event::InputEvent::getMaskForButton(4), 0);

    jxx::awt::event::KeyEvent event(
        nullptr,
        jxx::awt::event::KeyEvent::KEY_RELEASED,
        1,
        0,
        jxx::awt::event::KeyEvent::VK_UNDEFINED,
        jxx::awt::event::KeyEvent::CHAR_UNDEFINED);
    EXPECT_FALSE(event.isConsumed());
    event.consume();
    EXPECT_TRUE(event.isConsumed());
}
