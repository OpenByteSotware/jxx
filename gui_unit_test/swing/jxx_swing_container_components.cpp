#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.JLabel.h"
#include "swing/jxx.swing.JProgressBar.h"
#include "swing/jxx.swing.JSlider.h"
#include "swing/jxx.swing.JSplitPane.h"
#include "swing/jxx.swing.JTabbedPane.h"

namespace
{
    jxx::Ptr<jxx::lang::String> text(const char* value)
    {
        return jxx::NEW<jxx::lang::String>(value);
    }
}

TEST(SwingProgressBarTest, RangeStringAndIndeterminateStateRoundTrip)
{
    auto progress = jxx::NEW<jxx::swing::JProgressBar>(0, 200);
    progress->setValue(50);
    progress->setStringPainted(true);
    progress->setString(text("25%"));
    progress->setIndeterminate(true);
    EXPECT_EQ(progress->getValue(), 50);
    EXPECT_TRUE(progress->isStringPainted());
    EXPECT_EQ(progress->getString()->utf8(), "25%");
    EXPECT_TRUE(progress->isIndeterminate());
    EXPECT_DOUBLE_EQ(progress->getPercentComplete(), 0.25);
}

TEST(SwingSliderTest, TickAndPresentationStateRoundTrip)
{
    auto slider = jxx::NEW<jxx::swing::JSlider>(0, 100, 20);
    slider->setMajorTickSpacing(25);
    slider->setMinorTickSpacing(5);
    slider->setPaintTicks(true);
    slider->setSnapToTicks(true);
    slider->setInverted(true);
    slider->setPaintLabels(true);
    EXPECT_EQ(slider->getValue(), 20);
    EXPECT_EQ(slider->getMajorTickSpacing(), 25);
    EXPECT_EQ(slider->getMinorTickSpacing(), 5);
    EXPECT_TRUE(slider->getPaintTicks());
    EXPECT_TRUE(slider->getSnapToTicks());
    EXPECT_TRUE(slider->getInverted());
    EXPECT_TRUE(slider->getPaintLabels());
}

TEST(SwingTabbedPaneTest, TabsAndSelectionAreManagedHeadlessly)
{
    auto tabs = jxx::NEW<jxx::swing::JTabbedPane>();
    auto first = jxx::NEW<jxx::swing::JLabel>(text("First body"));
    auto second = jxx::NEW<jxx::swing::JLabel>(text("Second body"));
    tabs->addTab(text("First"), first);
    tabs->addTab(text("Second"), second);
    EXPECT_EQ(tabs->getTabCount(), 2);
    EXPECT_EQ(tabs->getSelectedIndex(), 0);
    tabs->setSelectedIndex(1);
    EXPECT_EQ(tabs->getSelectedComponent(), second);
    EXPECT_EQ(tabs->getTitleAt(1)->utf8(), "Second");
}

TEST(SwingSplitPaneTest, ComponentsAndDividerPropertiesRoundTrip)
{
    auto first = jxx::NEW<jxx::swing::JLabel>(text("Left"));
    auto second = jxx::NEW<jxx::swing::JLabel>(text("Right"));
    auto split = jxx::NEW<jxx::swing::JSplitPane>(
        jxx::swing::SwingConstants::HORIZONTAL, first, second);
    split->setDividerLocation(120);
    split->setDividerSize(8);
    split->setResizeWeight(0.4);
    split->setContinuousLayout(true);
    EXPECT_EQ(split->getLeftComponent(), first);
    EXPECT_EQ(split->getRightComponent(), second);
    EXPECT_EQ(split->getDividerLocation(), 120);
    EXPECT_EQ(split->getDividerSize(), 8);
    EXPECT_DOUBLE_EQ(split->getResizeWeight(), 0.4);
    EXPECT_TRUE(split->isContinuousLayout());
}
