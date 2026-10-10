#include <gtest/gtest.h>
#include "util/function/jxx.util.function.IntPredicate.h"
namespace {
class IntRule final : public ::jxx::lang::ClassBase<IntRule, ::jxx::lang::Object, ::jxx::util::function::IntPredicate> {
public:
    IntRule(::jxx::lang::jint threshold, int* calls) : threshold_(threshold), calls_(calls) {}
    ::jxx::lang::jbool test(::jxx::lang::jint value) override { ++*calls_; return value > threshold_; }
private: ::jxx::lang::jint threshold_; int* calls_;
};
}
TEST(JxxPrimitivePredicateParity, CompositionAndShortCircuit) {
    int firstCalls=0, secondCalls=0;
    auto first=::jxx::CAST<::jxx::util::function::IntPredicate>(::jxx::NEW<IntRule>(10,&firstCalls));
    auto second=::jxx::CAST<::jxx::util::function::IntPredicate>(::jxx::NEW<IntRule>(0,&secondCalls));
    EXPECT_FALSE(first->and_(second)->test(5));
    EXPECT_EQ(firstCalls,1); EXPECT_EQ(secondCalls,0);
    EXPECT_TRUE(first->or_(second)->test(5));
    EXPECT_FALSE(first->negate()->test(20));
}
TEST(JxxPrimitivePredicateParity, RejectsNullOperand) {
    int calls=0;
    auto rule=::jxx::CAST<::jxx::util::function::IntPredicate>(::jxx::NEW<IntRule>(0,&calls));
    EXPECT_THROW(rule->and_(nullptr),::jxx::lang::NullPointerException);
    EXPECT_THROW(rule->or_(nullptr),::jxx::lang::NullPointerException);
}
