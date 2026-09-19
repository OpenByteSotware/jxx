#include <gtest/gtest.h>

#include "swing/jxx.swing.DefaultBoundedRangeModel.h"
#include "swing/jxx.swing.DefaultListSelectionModel.h"
#include "swing/jxx.swing.ListSelectionModel.h"

TEST(DefaultBoundedRangeModelTest, DefaultsMatchExpectedContract)
{
    jxx::swing::DefaultBoundedRangeModel model;
    EXPECT_EQ(model.getMinimum(), 0);
    EXPECT_EQ(model.getMaximum(), 100);
    EXPECT_EQ(model.getValue(), 0);
    EXPECT_EQ(model.getExtent(), 0);
    EXPECT_FALSE(model.getValueIsAdjusting());
}

TEST(DefaultBoundedRangeModelTest, RangePropertiesRemainConsistent)
{
    jxx::swing::DefaultBoundedRangeModel model(25, 10, 0, 100);
    model.setRangeProperties(40, 15, 5, 80, true);
    EXPECT_EQ(model.getValue(), 40);
    EXPECT_EQ(model.getExtent(), 15);
    EXPECT_EQ(model.getMinimum(), 5);
    EXPECT_EQ(model.getMaximum(), 80);
    EXPECT_TRUE(model.getValueIsAdjusting());
}

TEST(DefaultListSelectionModelTest, IntervalOperationsTrackSelection)
{
    jxx::swing::DefaultListSelectionModel model;
    EXPECT_TRUE(model.isSelectionEmpty());
    model.setSelectionInterval(2, 4);
    EXPECT_EQ(model.getMinSelectionIndex(), 2);
    EXPECT_EQ(model.getMaxSelectionIndex(), 4);
    EXPECT_TRUE(model.isSelectedIndex(3));
    model.removeSelectionInterval(3, 3);
    EXPECT_FALSE(model.isSelectedIndex(3));
    EXPECT_TRUE(model.isSelectedIndex(2));
    EXPECT_TRUE(model.isSelectedIndex(4));
    model.clearSelection();
    EXPECT_TRUE(model.isSelectionEmpty());
}

TEST(DefaultListSelectionModelTest, SelectionModeRoundTrips)
{
    jxx::swing::DefaultListSelectionModel model;
    model.setSelectionMode(jxx::swing::ListSelectionModel::SINGLE_SELECTION);
    EXPECT_EQ(model.getSelectionMode(), jxx::swing::ListSelectionModel::SINGLE_SELECTION);
}
