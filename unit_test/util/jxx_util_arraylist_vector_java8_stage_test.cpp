#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Vector.h"
#include "util/function/jxx.util.function.PredicateSuper.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

namespace {
using S = ::jxx::lang::String;

class RemoveB final : public ::jxx::util::function::PredicateSuper<S> {
public:
    ::jxx::lang::jbool test(const ::jxx::Ptr<::jxx::lang::String>&value) override {
        return value->utf8() == "b";
    }
};

class Upper final : public ::jxx::util::function::UnaryOperator<S> {
public:
    ::jxx::Ptr<S> apply(const ::jxx::Ptr<S> value) override {
        return ::jxx::NEW<S>(
            value->utf8() == "a" ? "A" : value->utf8());
    }
};

TEST(ArrayListJava8StageTest, RemoveIfReplaceAllAndNaturalSort) {
    const auto list = ::jxx::NEW<::jxx::util::ArrayList<S>>();
    list->add(::jxx::NEW<S>("b"));
    list->add(::jxx::NEW<S>("a"));

    EXPECT_TRUE(list->removeIf(::jxx::NEW<RemoveB>()));
    list->replaceAll(::jxx::NEW<Upper>());

    const ::jxx::Ptr<::jxx::util::ComparatorSuper<S>> natural;
    list->sort(natural);

    EXPECT_TRUE(list->get(0)->utf8() == "A");
}

TEST(VectorLegacyStageTest, CopyIntoPreservesElements) {
    const auto vector = ::jxx::NEW<::jxx::util::Vector<S>>();
    vector->add(::jxx::NEW<S>("a"));
    vector->add(::jxx::NEW<S>("b"));

    const auto target = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<S>, 1U>>(3);
    vector->copyInto(target);

    ASSERT_NE(nullptr, (*target)[0]);
    ASSERT_NE(nullptr, (*target)[1]);
    EXPECT_TRUE((*target)[0]->utf8() == "a");
    EXPECT_TRUE((*target)[1]->utf8() == "b");
    EXPECT_EQ(nullptr, (*target)[2]);
}

} // namespace
