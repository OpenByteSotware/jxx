#include <gtest/gtest.h>

#include "swing/jxx.swing.JTable.h"

namespace
{
    ::jxx::Ptr<::jxx::swing::JTable> makeTable()
    {
        auto table = ::jxx::NEW<::jxx::swing::JTable>(3, 4);
        table->setRowSelectionAllowed(true);
        table->setColumnSelectionAllowed(true);
        return table;
    }
}

TEST(JTableKeyboardNavigationTest, ChangeSelectionMovesActiveCell)
{
    const auto table = makeTable();
    table->changeSelection(0, 0, false, false);
    EXPECT_EQ(0, table->getSelectedRow());
    EXPECT_EQ(0, table->getSelectedColumn());

    table->changeSelection(0, 1, false, false);
    EXPECT_EQ(0, table->getSelectedRow());
    EXPECT_EQ(1, table->getSelectedColumn());

    table->changeSelection(1, 1, false, false);
    EXPECT_EQ(1, table->getSelectedRow());
    EXPECT_EQ(1, table->getSelectedColumn());
}

TEST(JTableKeyboardNavigationTest, TabStyleNavigationWrapsAcrossRows)
{
    const auto table = makeTable();
    table->changeSelection(0, 3, false, false);

    const auto nextColumn = 0;
    const auto nextRow = table->getSelectedRow() + 1;
    table->changeSelection(nextRow, nextColumn, false, false);

    EXPECT_EQ(1, table->getSelectedRow());
    EXPECT_EQ(0, table->getSelectedColumn());
}

TEST(JTableKeyboardNavigationTest, ShiftTabStyleNavigationWrapsBackward)
{
    const auto table = makeTable();
    table->changeSelection(1, 0, false, false);
    table->changeSelection(0, 3, false, false);
    EXPECT_EQ(0, table->getSelectedRow());
    EXPECT_EQ(3, table->getSelectedColumn());
}

TEST(JTableKeyboardNavigationTest, ToggleClearsCurrentCell)
{
    const auto table = makeTable();
    table->changeSelection(1, 2, false, false);
    table->changeSelection(1, 2, true, false);
    EXPECT_EQ(-1, table->getSelectedRow());
    EXPECT_EQ(-1, table->getSelectedColumn());
}

TEST(JTableKeyboardNavigationTest, InvalidTargetThrows)
{
    const auto table = makeTable();
    EXPECT_THROW(table->changeSelection(-1, 0, false, false),
        ::jxx::lang::IndexOutOfBoundsException);
    EXPECT_THROW(table->changeSelection(0, 4, false, false),
        ::jxx::lang::IndexOutOfBoundsException);
}
