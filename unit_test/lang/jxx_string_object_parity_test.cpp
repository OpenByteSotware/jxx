#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringCaseInsensitiveComparator.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace {

class FinalizeProbe final : public jxx::lang::Object {
public:
    static int destructorCount;
    static int finalizeCount;

    ~FinalizeProbe() override {
        ++destructorCount;
    }

    void finalize() override {
        ++finalizeCount;
    }
};

int FinalizeProbe::destructorCount = 0;
int FinalizeProbe::finalizeCount = 0;

TEST(JxxStringParity, CaseInsensitiveOrderUsesStringComparison) {
    const auto lower = jxx::NEW<jxx::lang::String>("alpha");
    const auto upper = jxx::NEW<jxx::lang::String>("ALPHA");
    const auto beta = jxx::NEW<jxx::lang::String>("beta");

    ASSERT_NE(nullptr, jxx::lang::String::CASE_INSENSITIVE_ORDER);
    EXPECT_EQ(0, jxx::lang::String::CASE_INSENSITIVE_ORDER->compare(lower, upper));
    EXPECT_LT(jxx::lang::String::CASE_INSENSITIVE_ORDER->compare(lower, beta), 0);
    EXPECT_THROW(
        jxx::lang::String::CASE_INSENSITIVE_ORDER->compare(nullptr, beta),
        jxx::lang::NullPointerException);
}

TEST(JxxStringParity, ComparatorHasClassMetadata) {
    const auto comparator = jxx::CAST<jxx::lang::StringCaseInsensitiveComparator>(
        jxx::lang::String::CASE_INSENSITIVE_ORDER);
    ASSERT_NE(nullptr, comparator);
    EXPECT_NE(nullptr, comparator->getClass());
}

TEST(JxxObjectParity, ExplicitFinalizeUsesVirtualDispatch) {
    FinalizeProbe::finalizeCount = 0;
    auto probe = jxx::NEW<FinalizeProbe>();

    probe->finalize();

    EXPECT_EQ(1, FinalizeProbe::finalizeCount);
}

TEST(JxxObjectParity, DestructionDoesNotInvokeFinalize) {
    FinalizeProbe::destructorCount = 0;
    FinalizeProbe::finalizeCount = 0;
    {
        auto probe = jxx::NEW<FinalizeProbe>();
        ASSERT_NE(nullptr, probe);
    }

    EXPECT_EQ(1, FinalizeProbe::destructorCount);
    EXPECT_EQ(0, FinalizeProbe::finalizeCount);
}

} // namespace
