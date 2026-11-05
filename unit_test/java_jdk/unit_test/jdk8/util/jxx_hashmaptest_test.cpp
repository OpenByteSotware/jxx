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
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::util {
    TEST(HashMapTest, PutGetRemoveAndNullSupport) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> m = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        EXPECT_TRUE(m->isEmpty());
        EXPECT_EQ(m->put(::jxx::NEW<::jxx::lang::String>("a"), 1), nullptr);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(1), m->put(::jxx::NEW<::jxx::lang::String>("a"), 2)));
        m->put(nullptr, 3);
        m->put(::jxx::NEW<::jxx::lang::String>("nullValue"), nullptr);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(2), m->get(::jxx::NEW<::jxx::lang::String>("a"))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(3), m->get(nullptr)));
        EXPECT_TRUE(m->containsKey(::jxx::NEW<::jxx::lang::String>("nullValue")));
        EXPECT_TRUE(m->containsValue(nullptr));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(2), m->remove(::jxx::NEW<::jxx::lang::String>("a"))));
        EXPECT_FALSE(m->containsKey(::jxx::NEW<::jxx::lang::String>("a")));
    }

    TEST(HashMapTest, ViewsAreBackedByMap) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> m = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        m->put(::jxx::NEW<::jxx::lang::String>("a"), 1);
        m->put(::jxx::NEW<::jxx::lang::String>("b"), 2);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::util::HashSet<::jxx::lang::String>>(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("b"))), m->keySet()));
        EXPECT_TRUE(m->values()->containsAll(::jxx::util::Arrays::asList(1, 2)));
        for (::jxx::lang::jint jxxIndex_e = 0; jxxIndex_e < m->entrySet()->size(); ++jxxIndex_e) {
            ::jxx::Ptr<::jxx::util::Map> e = m->entrySet()->get(jxxIndex_e);
            if (e->getKey()->equals(::jxx::NEW<::jxx::lang::String>("a"))) {
                e->setValue(9);
            }
        }
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(9), m->get(::jxx::NEW<::jxx::lang::String>("a"))));
        m->keySet()->remove(::jxx::NEW<::jxx::lang::String>("b"));
        EXPECT_FALSE(m->containsKey(::jxx::NEW<::jxx::lang::String>("b")));
    }

    TEST(HashMapTest, CopyCloneEqualityAndClear) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> a = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        a->put(::jxx::NEW<::jxx::lang::String>("x"), 1);
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> b = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>(a);
        EXPECT_EQ(a, b);
        EXPECT_EQ(a->hashCode(), b->hashCode());
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> c = ::jxx::CAST<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>(a->clone());
        EXPECT_EQ(a, c);
        EXPECT_NE(a.get(), c.get());
        c->clear();
        EXPECT_TRUE(c->isEmpty());
    }

    TEST(HashMapTest, Java8MapOperations) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> m = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(1), m->putIfAbsent(::jxx::NEW<::jxx::lang::String>("a"), 1)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(1), m->getOrDefault(::jxx::NEW<::jxx::lang::String>("a"), 9)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(9), m->getOrDefault(::jxx::NEW<::jxx::lang::String>("x"), 9)));
        EXPECT_TRUE(m->replace(::jxx::NEW<::jxx::lang::String>("a"), 1, 2));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(2), m->replace(::jxx::NEW<::jxx::lang::String>("a"), 3)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(4), m->computeIfAbsent(::jxx::NEW<::jxx::lang::String>("b"), [&](auto k) {
    4;
})));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(5), m->computeIfPresent(::jxx::NEW<::jxx::lang::String>("b"), [&](auto k, auto v) {
    (v + 1);
})));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(1), m->compute(::jxx::NEW<::jxx::lang::String>("c"), [&](auto k, auto v) {
    1;
})));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(7), m->merge(::jxx::NEW<::jxx::lang::String>("c"), 6, [&](auto x, auto y) {
    (x + y);
})));
        m->replaceAll([&](auto k, auto v) {
    (v * 2);
});
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(14), m->get(::jxx::NEW<::jxx::lang::String>("c"))));
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> sum = ::jxx::NEW<::jxx::lang::IntArrayType>(1U);
        (*sum)[0U] = 0;
        m->forEach([&](auto k, auto v) {
    (*sum)[0] += v;
});
        EXPECT_EQ(24, (*sum)[0]);
        EXPECT_TRUE(m->remove(::jxx::NEW<::jxx::lang::String>("a"), 6));
    }

} // namespace jxx::jdk8::util
