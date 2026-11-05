#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::util {
    TEST(ArrayListTest, ConstructionAndBasicOperations) {
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::String>> x = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
        EXPECT_TRUE(x->isEmpty());
        EXPECT_TRUE(x->add(::jxx::NEW<::jxx::lang::String>("a")));
        x->add(0, ::jxx::NEW<::jxx::lang::String>("z"));
        EXPECT_EQ(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("z"), ::jxx::NEW<::jxx::lang::String>("a")), x);
        EXPECT_EQ(2, x->size());
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("a"), x->get(1)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("a"), x->set(1, ::jxx::NEW<::jxx::lang::String>("b"))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("b"), x->get(1)));
    }

    TEST(ArrayListTest, CollectionOperations) {
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::String>> x = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("b"), ::jxx::NEW<::jxx::lang::String>("c"), ::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_TRUE(x->contains(::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_EQ(1, x->indexOf(::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_EQ(3, x->lastIndexOf(::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_TRUE(x->remove(::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("c"), x->remove(1)));
        EXPECT_TRUE(x->containsAll(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("b"))));
        x->addAll(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("d"), ::jxx::NEW<::jxx::lang::String>("e")));
        x->addAll(1, ::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("x"), ::jxx::NEW<::jxx::lang::String>("y")));
        EXPECT_EQ(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("x"), ::jxx::NEW<::jxx::lang::String>("y"), ::jxx::NEW<::jxx::lang::String>("b"), ::jxx::NEW<::jxx::lang::String>("d"), ::jxx::NEW<::jxx::lang::String>("e")), x);
    }

    TEST(ArrayListTest, BulkAndArrayOperations) {
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::String>> x = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("b"), ::jxx::NEW<::jxx::lang::String>("c")));
        EXPECT_TRUE(x->removeAll(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("b"), ::jxx::NEW<::jxx::lang::String>("z"))));
        EXPECT_TRUE(x->retainAll(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("z"))));
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>> a = x->toArray();
        assertArrayEquals(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>(), a);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>> s = x->toArray(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(0));
        assertArrayEquals(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(), s);
        x->clear();
        EXPECT_TRUE(x->isEmpty());
    }

    TEST(ArrayListTest, IterationSublistAndClone) {
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::Integer>> x = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(1, 2, 3));
        ::jxx::lang::jint sum = 0;
        for (::jxx::lang::jint jxxIndex_n = 0; jxxIndex_n < x->size(); ++jxxIndex_n) {
            ::jxx::Ptr<::jxx::lang::Integer> n = x->get(jxxIndex_n);
            sum += n;
        }
        EXPECT_EQ(6, sum);
        ::jxx::Ptr<::jxx::util::ListIterator<::jxx::lang::Integer>> it = x->listIterator(1);
        EXPECT_TRUE(it->hasNext());
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(2), ::jxx::lang::String::valueOf(it->next())));
        EXPECT_EQ(::jxx::util::Arrays::asList(2, 3), x->subList(1, 3));
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::Integer>> copy = ::jxx::CAST<::jxx::util::ArrayList<::jxx::lang::Integer>>(x->clone());
        EXPECT_EQ(x, copy);
        EXPECT_NE(x.get(), copy.get());
    }

    TEST(ArrayListTest, Java8Operations) {
        ::jxx::Ptr<::jxx::util::ArrayList<::jxx::lang::Integer>> x = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(3, 1, 2));
        x->removeIf([&](auto n) {
    (n == 1);
});
        x->replaceAll([&](auto n) {
    (n * 2);
});
        x->sort(nullptr);
        EXPECT_EQ(::jxx::util::Arrays::asList(4, 6), x);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> sum = ::jxx::NEW<::jxx::lang::IntArrayType>(1U);
        (*sum)[0U] = 0;
        x->forEach([&](auto n) {
    (*sum)[0] += n;
});
        EXPECT_EQ(10, (*sum)[0]);
        EXPECT_NE(x->spliterator(), nullptr);
    }

    TEST(ArrayListTest, GetRejectsInvalidIndex) {
        EXPECT_THROW(
            {
                (::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>())->get(0);
            },
            ::jxx::lang::IndexOutOfBoundsException);
    }

} // namespace jxx::jdk8::util
