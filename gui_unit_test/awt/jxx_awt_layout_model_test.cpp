#include <gtest/gtest.h>

#include "awt/jxx.awt.FlowLayout.h"
#include "awt/jxx.awt.GridLayout.h"

TEST(GridLayoutTest, ConstructorAndMutatorsExposeConfiguration)
{
    jxx::awt::GridLayout layout(2, 3, 4, 5);
    EXPECT_EQ(layout.getRows(), 2);
    EXPECT_EQ(layout.getColumns(), 3);
    EXPECT_EQ(layout.getHgap(), 4);
    EXPECT_EQ(layout.getVgap(), 5);
    layout.setRows(4);
    layout.setColumns(1);
    layout.setHgap(6);
    layout.setVgap(7);
    EXPECT_EQ(layout.getRows(), 4);
    EXPECT_EQ(layout.getColumns(), 1);
    EXPECT_EQ(layout.getHgap(), 6);
    EXPECT_EQ(layout.getVgap(), 7);
}

TEST(FlowLayoutTest, AlignmentAndGapPropertiesRoundTrip)
{
    jxx::awt::FlowLayout layout(jxx::awt::FlowLayout::LEFT, 8, 9);
    EXPECT_EQ(layout.getAlignment(), jxx::awt::FlowLayout::LEFT);
    EXPECT_EQ(layout.getHgap(), 8);
    EXPECT_EQ(layout.getVgap(), 9);
    layout.setAlignment(jxx::awt::FlowLayout::RIGHT);
    layout.setHgap(3);
    layout.setVgap(4);
    EXPECT_EQ(layout.getAlignment(), jxx::awt::FlowLayout::RIGHT);
    EXPECT_EQ(layout.getHgap(), 3);
    EXPECT_EQ(layout.getVgap(), 4);
}
