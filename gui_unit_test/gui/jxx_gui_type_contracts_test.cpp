#include <gtest/gtest.h>
#include <type_traits>

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.LayoutManager.h"
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.BoundedRangeModel.h"
#include "swing/jxx.swing.DefaultBoundedRangeModel.h"
#include "swing/jxx.swing.JButton.h"
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.JPanel.h"

static_assert(std::is_base_of_v<jxx::lang::Object, jxx::awt::Dimension>);
static_assert(std::is_base_of_v<jxx::swing::JComponent, jxx::swing::JButton>);
static_assert(std::is_base_of_v<jxx::swing::JComponent, jxx::swing::JPanel>);
static_assert(std::is_base_of_v<jxx::swing::BoundedRangeModel,
    jxx::swing::DefaultBoundedRangeModel>);
static_assert(std::is_abstract_v<jxx::awt::LayoutManager>);
static_assert(std::is_abstract_v<jxx::swing::BoundedRangeModel>);

TEST(TypeContractsTest, CompileTimeContractsAreSatisfied)
{
    SUCCEED();
}
