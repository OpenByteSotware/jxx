#include <gtest/gtest.h>

#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Checkbox.h"
#include "awt/jxx.awt.Choice.h"
#include "awt/jxx.awt.Label.h"
#include "awt/jxx.awt.Scrollbar.h"
#include "awt/jxx.awt.TextField.h"
#include "lang/jxx.lang.String.h"

namespace
{
    jxx::Ptr<jxx::lang::String> text(const char* value)
    {
        return jxx::NEW<jxx::lang::String>(value);
    }
}

TEST(AwtButtonTest, LabelAndActionCommandRoundTrip)
{
    auto button = jxx::NEW<jxx::awt::Button>(text("Run"));
    EXPECT_EQ(button->getLabel()->utf8(), "Run");
    EXPECT_EQ(button->getActionCommand()->utf8(), "Run");
    button->setLabel(text("Start"));
    button->setActionCommand(text("start-command"));
    EXPECT_EQ(button->getLabel()->utf8(), "Start");
    EXPECT_EQ(button->getActionCommand()->utf8(), "start-command");
}

TEST(AwtLabelTest, TextAndAlignmentRoundTrip)
{
    auto label = jxx::NEW<jxx::awt::Label>(text("Status"), jxx::awt::Label::CENTER);
    EXPECT_EQ(label->getText()->utf8(), "Status");
    EXPECT_EQ(label->getAlignment(), jxx::awt::Label::CENTER);
    label->setText(text("Ready"));
    label->setAlignment(jxx::awt::Label::RIGHT);
    EXPECT_EQ(label->getText()->utf8(), "Ready");
    EXPECT_EQ(label->getAlignment(), jxx::awt::Label::RIGHT);
}

TEST(AwtTextFieldTest, TextColumnsAndEchoCharacterRoundTrip)
{
    auto field = jxx::NEW<jxx::awt::TextField>(text("value"), 12);
    EXPECT_EQ(field->getText()->utf8(), "value");
    EXPECT_EQ(field->getColumns(), 12);
    field->setColumns(18);
    field->setEchoChar(static_cast<jxx::lang::jchar>('*'));
    EXPECT_EQ(field->getColumns(), 18);
    EXPECT_TRUE(field->echoCharIsSet());
    EXPECT_EQ(field->getEchoChar(), static_cast<jxx::lang::jchar>('*'));
}

TEST(AwtCheckboxTest, SelectionControlsSelectedObjects)
{
    auto checkbox = jxx::NEW<jxx::awt::Checkbox>(text("Enabled"), false);
    EXPECT_FALSE(checkbox->getState());
    EXPECT_EQ(checkbox->getSelectedObjects(), nullptr);
    checkbox->setState(true);
    EXPECT_TRUE(checkbox->getState());
    const auto selected = checkbox->getSelectedObjects();
    ASSERT_NE(selected, nullptr);
    ASSERT_EQ(selected->length, 1u);
}

TEST(AwtChoiceTest, FirstItemIsSelectedAndSelectionCanChange)
{
    auto choice = jxx::NEW<jxx::awt::Choice>();
    choice->add(text("one"));
    choice->add(text("two"));
    EXPECT_EQ(choice->getItemCount(), 2);
    EXPECT_EQ(choice->getSelectedIndex(), 0);
    choice->select(1);
    EXPECT_EQ(choice->getSelectedIndex(), 1);
    EXPECT_EQ(choice->getSelectedItem()->utf8(), "two");
}

TEST(AwtScrollbarTest, RangeAndIncrementStateRoundTrip)
{
    auto scrollbar = jxx::NEW<jxx::awt::Scrollbar>(
        jxx::awt::Scrollbar::HORIZONTAL, 30, 10, 0, 100);
    EXPECT_EQ(scrollbar->getOrientation(), jxx::awt::Scrollbar::HORIZONTAL);
    EXPECT_EQ(scrollbar->getValue(), 30);
    EXPECT_EQ(scrollbar->getVisibleAmount(), 10);
    scrollbar->setUnitIncrement(2);
    scrollbar->setBlockIncrement(20);
    scrollbar->setValueIsAdjusting(true);
    EXPECT_EQ(scrollbar->getUnitIncrement(), 2);
    EXPECT_EQ(scrollbar->getBlockIncrement(), 20);
    EXPECT_TRUE(scrollbar->getValueIsAdjusting());
}
