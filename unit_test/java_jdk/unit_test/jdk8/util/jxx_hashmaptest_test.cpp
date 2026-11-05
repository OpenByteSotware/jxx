#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Objects.h"

namespace {
template<typename T, typename F> class JxxLambdaConsumer final : public ::jxx::lang::ClassBase<JxxLambdaConsumer<T,F>, ::jxx::lang::Object, ::jxx::util::function::Consumer<T>> { public: explicit JxxLambdaConsumer(F f):f_(std::move(f)){} void accept(const ::jxx::Ptr<T>& v) override { f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakeConsumer(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Consumer<T>>(::jxx::NEW<JxxLambdaConsumer<T,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaPredicate final : public ::jxx::lang::ClassBase<JxxLambdaPredicate<T,F>, ::jxx::lang::Object, ::jxx::util::function::Predicate<T>> { public: explicit JxxLambdaPredicate(F f):f_(std::move(f)){} ::jxx::lang::jbool test(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakePredicate(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Predicate<T>>(::jxx::NEW<JxxLambdaPredicate<T,D>>(std::forward<F>(f))); }
template<typename T, typename R, typename F> class JxxLambdaFunction final : public ::jxx::lang::ClassBase<JxxLambdaFunction<T,R,F>, ::jxx::lang::Object, ::jxx::util::function::Function<T,R>> { public: explicit JxxLambdaFunction(F f):f_(std::move(f)){} ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename R, typename F> auto jxxMakeFunction(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Function<T,R>>(::jxx::NEW<JxxLambdaFunction<T,R,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaSupplier final : public ::jxx::lang::ClassBase<JxxLambdaSupplier<T,F>, ::jxx::lang::Object, ::jxx::util::function::Supplier<T>> { public: explicit JxxLambdaSupplier(F f):f_(std::move(f)){} ::jxx::Ptr<T> get() override { return f_(); } private: F f_; };
template<typename T, typename F> auto jxxMakeSupplier(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Supplier<T>>(::jxx::NEW<JxxLambdaSupplier<T,D>>(std::forward<F>(f))); }
} // namespace

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
            ::jxx::Ptr<::jxx::util::MapEntry<::jxx::lang::String, ::jxx::lang::Integer>> e = m->entrySet()->get(jxxIndex_e);
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
        EXPECT_TRUE(::jxx::util::Objects::equals(a, b));
        EXPECT_EQ(a->hashCode(), b->hashCode());
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> c = ::jxx::CAST<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>(a->clone());
        EXPECT_TRUE(::jxx::util::Objects::equals(a, c));
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
