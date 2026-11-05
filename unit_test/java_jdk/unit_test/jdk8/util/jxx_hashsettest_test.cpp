#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::util {
    TEST(HashSetTest, SetRejectsDuplicatesAndSupportsNull) {
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::String>> s = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::String>>();
        EXPECT_TRUE(s->add(::jxx::NEW<::jxx::lang::String>("a")));
        EXPECT_FALSE(s->add(::jxx::NEW<::jxx::lang::String>("a")));
        EXPECT_TRUE(s->add(nullptr));
        EXPECT_EQ(2, s->size());
        EXPECT_TRUE(s->contains(nullptr));
        EXPECT_TRUE(s->remove(::jxx::NEW<::jxx::lang::String>("a")));
        EXPECT_FALSE(s->contains(::jxx::NEW<::jxx::lang::String>("a")));
    }

    TEST(HashSetTest, CollectionConstructorAndBulkOperations) {
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> s = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(1, 2, 2, 3));
        EXPECT_EQ(3, s->size());
        EXPECT_TRUE(s->containsAll(::jxx::util::Arrays::asList(1, 2)));
        EXPECT_TRUE(s->addAll(::jxx::util::Arrays::asList(3, 4)));
        EXPECT_TRUE(s->removeAll(::jxx::util::Arrays::asList(1, 9)));
        EXPECT_TRUE(s->retainAll(::jxx::util::Arrays::asList(2, 4)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(2, 4)), s));
    }

    TEST(HashSetTest, CloneArrayIterationAndClear) {
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> s = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(1, 2, 3));
        ::jxx::lang::jint sum = 0;
        for (::jxx::lang::jint jxxIndex_n = 0; jxxIndex_n < s->size(); ++jxxIndex_n) {
            ::jxx::Ptr<::jxx::lang::Integer> n = s->get(jxxIndex_n);
            sum += n;
        }
        EXPECT_EQ(6, sum);
        EXPECT_EQ(3, s->toArray()->length);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Integer>, 1U>> a = s->toArray(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Integer>, 1U>>(0));
        EXPECT_EQ(3, a->length);
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> c = ::jxx::CAST<::jxx::util::HashSet<::jxx::lang::Integer>>(s->clone());
        EXPECT_EQ(s, c);
        EXPECT_NE(s.get(), c.get());
        EXPECT_NE(s->spliterator(), nullptr);
        s->clear();
        EXPECT_TRUE(s->isEmpty());
    }

    TEST(HashSetTest, RemoveIfAndForEachWork) {
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> s = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(1, 2, 3, 4));
        EXPECT_TRUE(s->removeIf([&](auto n) {
    ((n % 2) == 0);
}));
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> sum = ::jxx::NEW<::jxx::lang::IntArrayType>(1U);
        (*sum)[0U] = 0;
        s->forEach([&](auto n) {
    (*sum)[0] += n;
});
        EXPECT_EQ(4, (*sum)[0]);
    }

} // namespace jxx::jdk8::util
