#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.JButton.h"
#include "swing/jxx.swing.JCheckBox.h"
#include "swing/jxx.swing.JLabel.h"
#include "swing/jxx.swing.JRadioButton.h"
#include "swing/jxx.swing.JTextArea.h"
#include "swing/jxx.swing.JTextField.h"

namespace
{
    jxx::Ptr<jxx::lang::String> text(const char* value)
    {
        return jxx::NEW<jxx::lang::String>(value);
    }
}

TEST(SwingLabelTest, TextAndAlignmentRoundTripHeadlessly)
{
    auto label = jxx::NEW<jxx::swing::JLabel>(text("Name"), jxx::swing::SwingConstants::CENTER);
    EXPECT_EQ(label->getText()->utf8(), "Name");
    EXPECT_EQ(label->getHorizontalAlignment(), jxx::swing::SwingConstants::CENTER);
    label->setText(text("Updated"));
    EXPECT_EQ(label->getText()->utf8(), "Updated");
}

TEST(SwingButtonTest, TextCommandAndDefaultCapableStateRoundTrip)
{
    auto button = jxx::NEW<jxx::swing::JButton>(text("Apply"));
    EXPECT_EQ(button->getText()->utf8(), "Apply");
    button->setActionCommand(text("apply-command"));
    button->setDefaultCapable(false);
    EXPECT_EQ(button->getActionCommand()->utf8(), "apply-command");
    EXPECT_FALSE(button->isDefaultCapable());
}

TEST(SwingToggleButtonTest, ClickTogglesSelection)
{
    auto toggle = jxx::NEW<jxx::swing::JCheckBox>(text("Option"), false);
    EXPECT_FALSE(toggle->isSelected());
    toggle->doClick();
    EXPECT_TRUE(toggle->isSelected());
    toggle->doClick();
    EXPECT_FALSE(toggle->isSelected());
}

TEST(SwingRadioButtonTest, SelectionCanBeChangedProgrammatically)
{
    auto radio = jxx::NEW<jxx::swing::JRadioButton>(text("Choice"), true);
    EXPECT_TRUE(radio->isSelected());
    radio->setSelected(false);
    EXPECT_FALSE(radio->isSelected());
}

TEST(SwingTextFieldTest, TextColumnsAlignmentAndActionCommandRoundTrip)
{
    auto field = jxx::NEW<jxx::swing::JTextField>(text("abc"), 10);
    EXPECT_EQ(field->getText()->utf8(), "abc");
    EXPECT_EQ(field->getColumns(), 10);
    field->setHorizontalAlignment(jxx::swing::SwingConstants::RIGHT);
    field->setActionCommand(text("accept"));
    EXPECT_EQ(field->getHorizontalAlignment(), jxx::swing::SwingConstants::RIGHT);
    EXPECT_EQ(field->getActionCommand()->utf8(), "accept");
}

TEST(SwingTextAreaTest, TextMutationAndWrappingWorkHeadlessly)
{
    auto area = jxx::NEW<jxx::swing::JTextArea>(text("abc"), 3, 12);
    area->append(text("def"));
    EXPECT_EQ(area->getText()->utf8(), "abcdef");
    area->insert(text("-"), 3);
    EXPECT_EQ(area->getText()->utf8(), "abc-def");
    area->replaceRange(text("XYZ"), 0, 3);
    EXPECT_EQ(area->getText()->utf8(), "XYZ-def");
    area->setLineWrap(true);
    area->setWrapStyleWord(true);
    EXPECT_TRUE(area->getLineWrap());
    EXPECT_TRUE(area->getWrapStyleWord());
}
