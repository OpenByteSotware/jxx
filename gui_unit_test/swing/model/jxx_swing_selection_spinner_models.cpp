#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.DefaultSingleSelectionModel.h"
#include "swing/jxx.swing.SpinnerListModel.h"

TEST(DefaultSingleSelectionModelTest, SelectionStateRoundTrips)
{
    auto model = jxx::NEW<jxx::swing::DefaultSingleSelectionModel>();
    EXPECT_FALSE(model->isSelected());
    EXPECT_EQ(model->getSelectedIndex(), -1);
    model->setSelectedIndex(3);
    EXPECT_TRUE(model->isSelected());
    EXPECT_EQ(model->getSelectedIndex(), 3);
    model->clearSelection();
    EXPECT_FALSE(model->isSelected());
}

TEST(SpinnerListModelTest, CurrentNextAndPreviousValuesFollowListOrder)
{
    auto one = jxx::NEW<jxx::lang::String>("one");
    auto two = jxx::NEW<jxx::lang::String>("two");
    auto three = jxx::NEW<jxx::lang::String>("three");
    std::vector<jxx::Ptr<jxx::lang::Object>> values{
        jxx::CAST<jxx::lang::Object>(one),
        jxx::CAST<jxx::lang::Object>(two),
        jxx::CAST<jxx::lang::Object>(three)};
    auto model = jxx::NEW<jxx::swing::SpinnerListModel>(values);
    EXPECT_EQ(model->getValue(), values[0]);
    EXPECT_EQ(model->getNextValue(), values[1]);
    EXPECT_EQ(model->getPreviousValue(), nullptr);
    model->setValue(values[1]);
    EXPECT_EQ(model->getValue(), values[1]);
    EXPECT_EQ(model->getPreviousValue(), values[0]);
    EXPECT_EQ(model->getNextValue(), values[2]);
}
