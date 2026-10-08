#include <gtest/gtest.h>
#include "util/function/jxx.util.function.Predicate.h"
#include "lang/jxx.lang.String.h"

namespace {
class NonEmptyPredicate final
    : public ::jxx::lang::ClassBase<
          NonEmptyPredicate,
          ::jxx::lang::Object,
          ::jxx::util::function::Predicate<::jxx::lang::String>> {
public:
    ::jxx::lang::jbool test(
        const ::jxx::Ptr<::jxx::lang::String>& value) override {
        return value != nullptr && value->length() != 0;
    }
};
}

TEST(JxxPredicateJava8ParityTest, CompositionUsesReferenceSemantics) {
    using Predicate = ::jxx::util::function::Predicate<::jxx::lang::String>;
    const auto predicate = ::jxx::CAST<Predicate>(::jxx::NEW<NonEmptyPredicate>());
    const auto value = ::jxx::NEW<::jxx::lang::String>("value");
    EXPECT_TRUE(predicate->test(value));
    EXPECT_FALSE(predicate->negate()->test(value));
    EXPECT_TRUE(predicate->and_(predicate)->test(value));
    EXPECT_TRUE(predicate->or_(predicate->negate())->test(value));
}

TEST(JxxPredicateJava8ParityTest, NullOperandsAreRejected) {
    using Predicate = ::jxx::util::function::Predicate<::jxx::lang::String>;
    const auto predicate = ::jxx::CAST<Predicate>(::jxx::NEW<NonEmptyPredicate>());
    EXPECT_THROW(predicate->and_(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(predicate->or_(nullptr), ::jxx::lang::NullPointerException);
}
