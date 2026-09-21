#include <gtest/gtest.h>

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
#include "swing/jxx.swing.JTable.h"

TEST(JTableHeadlessTest, GeometryRequiresNoNativeWindow)
{
    const auto table = ::jxx::NEW<::jxx::swing::JTable>(2, 2);
    table->setRowHeight(20);
    table->setIntercellSpacing(
        ::jxx::NEW<::jxx::awt::Dimension>(2, 1));

    EXPECT_EQ(0, table->rowAtPoint(
        ::jxx::NEW<::jxx::awt::Point>(1, 19)));
    EXPECT_EQ(1, table->rowAtPoint(
        ::jxx::NEW<::jxx::awt::Point>(1, 20)));
    EXPECT_EQ(-1, table->rowAtPoint(
        ::jxx::NEW<::jxx::awt::Point>(1, 40)));

    const auto cell = table->getCellRect(1, 1, false);
    EXPECT_EQ(20, cell->y);
    EXPECT_GT(cell->width, 0);
    EXPECT_EQ(19, cell->height);
}

TEST(JTableHeadlessTest, SelectAllAndClearSelectionRemainDeterministic)
{
    const auto table = ::jxx::NEW<::jxx::swing::JTable>(3, 2);
    table->setRowSelectionAllowed(true);
    table->setColumnSelectionAllowed(true);
    table->selectAll();
    EXPECT_EQ(2, table->getSelectedRow());
    EXPECT_EQ(1, table->getSelectedColumn());

    table->clearSelection();
    EXPECT_EQ(-1, table->getSelectedRow());
    EXPECT_EQ(-1, table->getSelectedColumn());
}
