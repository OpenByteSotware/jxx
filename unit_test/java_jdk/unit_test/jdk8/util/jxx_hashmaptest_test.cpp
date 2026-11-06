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
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.wildcard.CollectionAdapters.h"

namespace {
template<typename T, typename F> class JxxLambdaConsumer final : public ::jxx::lang::ClassBase<JxxLambdaConsumer<T,F>, ::jxx::lang::Object, ::jxx::util::function::Consumer<T>> { public: explicit JxxLambdaConsumer(F f):f_(std::move(f)){} void accept(const ::jxx::Ptr<T>& v) override { f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakeConsumer(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Consumer<T>>(::jxx::NEW<JxxLambdaConsumer<T,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaPredicate final : public ::jxx::lang::ClassBase<JxxLambdaPredicate<T,F>, ::jxx::lang::Object, ::jxx::util::function::Predicate<T>> { public: explicit JxxLambdaPredicate(F f):f_(std::move(f)){} ::jxx::lang::jbool test(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakePredicate(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Predicate<T>>(::jxx::NEW<JxxLambdaPredicate<T,D>>(std::forward<F>(f))); }
template<typename T, typename R, typename F> class JxxLambdaFunction final : public ::jxx::lang::ClassBase<JxxLambdaFunction<T,R,F>, ::jxx::lang::Object, ::jxx::util::function::Function<T,R>> { public: explicit JxxLambdaFunction(F f):f_(std::move(f)){} ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename R, typename F> auto jxxMakeFunction(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Function<T,R>>(::jxx::NEW<JxxLambdaFunction<T,R,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaUnaryOperator final : public ::jxx::lang::ClassBase<JxxLambdaUnaryOperator<T,F>, ::jxx::lang::Object, ::jxx::util::function::UnaryOperator<T>> { public: explicit JxxLambdaUnaryOperator(F f):f_(std::move(f)){} ::jxx::Ptr<T> apply(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakeUnaryOperator(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::UnaryOperator<T>>(::jxx::NEW<JxxLambdaUnaryOperator<T,D>>(std::forward<F>(f))); }
template<typename T, typename U, typename R, typename F> class JxxLambdaBiFunction final : public ::jxx::lang::ClassBase<JxxLambdaBiFunction<T,U,R,F>, ::jxx::lang::Object, ::jxx::util::function::BiFunction<T,U,R>> { public: explicit JxxLambdaBiFunction(F f):f_(std::move(f)){} ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& a,const ::jxx::Ptr<U>& b) override { return f_(a,b); } private: F f_; };
template<typename T, typename U, typename R, typename F> auto jxxMakeBiFunction(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::BiFunction<T,U,R>>(::jxx::NEW<JxxLambdaBiFunction<T,U,R,D>>(std::forward<F>(f))); }
template<typename T, typename U, typename F> class JxxLambdaBiConsumer final : public ::jxx::lang::ClassBase<JxxLambdaBiConsumer<T,U,F>, ::jxx::lang::Object, ::jxx::util::function::BiConsumer<T,U>> { public: explicit JxxLambdaBiConsumer(F f):f_(std::move(f)){} void accept(const ::jxx::Ptr<T>& a,const ::jxx::Ptr<U>& b) override { f_(a,b); } private: F f_; };
template<typename T, typename U, typename F> auto jxxMakeBiConsumer(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::BiConsumer<T,U>>(::jxx::NEW<JxxLambdaBiConsumer<T,U,D>>(std::forward<F>(f))); }
template<typename F> class JxxLambdaIntUnaryOperator final : public ::jxx::lang::ClassBase<JxxLambdaIntUnaryOperator<F>, ::jxx::lang::Object, ::jxx::util::function::IntUnaryOperator> { public: explicit JxxLambdaIntUnaryOperator(F f):f_(std::move(f)){} ::jxx::lang::jint applyAsInt(::jxx::lang::jint v) override { return f_(v); } private: F f_; };
template<typename F> auto jxxMakeIntUnaryOperator(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::IntUnaryOperator>(::jxx::NEW<JxxLambdaIntUnaryOperator<D>>(std::forward<F>(f))); }
template<typename F> class JxxLambdaIntBinaryOperator final : public ::jxx::lang::ClassBase<JxxLambdaIntBinaryOperator<F>, ::jxx::lang::Object, ::jxx::util::function::IntBinaryOperator> { public: explicit JxxLambdaIntBinaryOperator(F f):f_(std::move(f)){} ::jxx::lang::jint applyAsInt(::jxx::lang::jint a, ::jxx::lang::jint b) override { return f_(a,b); } private: F f_; };
template<typename F> auto jxxMakeIntBinaryOperator(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::IntBinaryOperator>(::jxx::NEW<JxxLambdaIntBinaryOperator<D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaSupplier final : public ::jxx::lang::ClassBase<JxxLambdaSupplier<T,F>, ::jxx::lang::Object, ::jxx::util::function::Supplier<T>> { public: explicit JxxLambdaSupplier(F f):f_(std::move(f)){} ::jxx::Ptr<T> get() override { return f_(); } private: F f_; };
template<typename T, typename F> auto jxxMakeSupplier(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Supplier<T>>(::jxx::NEW<JxxLambdaSupplier<T,D>>(std::forward<F>(f))); }
} // namespace

namespace jxx::jdk8::util {
    TEST(HashMapTest, PutGetRemoveAndNullSupport) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> m = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        EXPECT_TRUE(m->isEmpty());
        EXPECT_EQ(m->put(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(1)), nullptr);
        EXPECT_EQ(::jxx::lang::Integer::valueOf(1), m->put(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(2)));
        m->put(nullptr, ::jxx::lang::Integer::valueOf(3));
        m->put(::jxx::NEW<::jxx::lang::String>("nullValue"), nullptr);
        EXPECT_EQ(::jxx::lang::Integer::valueOf(2), m->get(::jxx::NEW<::jxx::lang::String>("a")));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(3), m->get(nullptr));
        EXPECT_TRUE(m->containsKey(::jxx::NEW<::jxx::lang::String>("nullValue")));
        EXPECT_TRUE(m->containsValue(nullptr));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(2), m->remove(::jxx::NEW<::jxx::lang::String>("a")));
        EXPECT_FALSE(m->containsKey(::jxx::NEW<::jxx::lang::String>("a")));
    }

    TEST(HashMapTest, ViewsAreBackedByMap) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> m = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        m->put(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(1));
        m->put(::jxx::NEW<::jxx::lang::String>("b"), ::jxx::lang::Integer::valueOf(2));
        EXPECT_EQ(::jxx::NEW<::jxx::util::HashSet<::jxx::lang::String>>(::jxx::CAST<::jxx::util::wildcard::CollectionExtends<::jxx::lang::String>>(::jxx::NEW<::jxx::util::wildcard::CollectionExtendsView<::jxx::lang::String, ::jxx::lang::String>>(::jxx::CAST<::jxx::util::Collection<::jxx::lang::String>>(::jxx::util::Arrays::asList(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::NEW<::jxx::lang::String>("b")))))), m->keySet());
        EXPECT_TRUE(m->values()->containsAll(::jxx::CAST<::jxx::util::wildcard::CollectionAny>(::jxx::NEW<::jxx::util::wildcard::CollectionAnyView<::jxx::lang::Integer>>(::jxx::CAST<::jxx::util::Collection<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(::jxx::lang::Integer::valueOf(1), ::jxx::lang::Integer::valueOf(2)))))));
        for (auto jxxIterator_e = m->entrySet()->iterator(); jxxIterator_e->hasNext();) {
            ::jxx::Ptr<::jxx::util::MapEntry<::jxx::lang::String, ::jxx::lang::Integer>> e = jxxIterator_e->next();
            if (e->getKey()->equals(::jxx::NEW<::jxx::lang::String>("a"))) {
                e->setValue(::jxx::lang::Integer::valueOf(9));
            }
        }
        EXPECT_EQ(::jxx::lang::Integer::valueOf(9), m->get(::jxx::NEW<::jxx::lang::String>("a")));
        m->keySet()->remove(::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<::jxx::lang::String>("b")));
        EXPECT_FALSE(m->containsKey(::jxx::NEW<::jxx::lang::String>("b")));
    }

    TEST(HashMapTest, CopyCloneEqualityAndClear) {
        ::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>> a = ::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Integer>>();
        a->put(::jxx::NEW<::jxx::lang::String>("x"), ::jxx::lang::Integer::valueOf(1));
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
        EXPECT_EQ(::jxx::lang::Integer::valueOf(1), m->putIfAbsent(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(1)));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(1), m->getOrDefault(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(9)));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(9), m->getOrDefault(::jxx::NEW<::jxx::lang::String>("x"), ::jxx::lang::Integer::valueOf(9)));
        EXPECT_TRUE(m->replace(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(1), ::jxx::lang::Integer::valueOf(2)));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(2), m->replace(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(3)));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(4), m->computeIfAbsent(::jxx::NEW<::jxx::lang::String>("b"), jxxMakeFunction<::jxx::lang::String, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& k) {
    return ::jxx::lang::Integer::valueOf(4);
})));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(5), m->computeIfPresent(::jxx::NEW<::jxx::lang::String>("b"), jxxMakeBiFunction<::jxx::lang::String, ::jxx::lang::Integer, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& k, const ::jxx::Ptr<::jxx::lang::Integer>& v) {
    return ::jxx::lang::Integer::valueOf(((v)->intValue() + 1));
})));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(1), m->compute(::jxx::NEW<::jxx::lang::String>("c"), jxxMakeBiFunction<::jxx::lang::String, ::jxx::lang::Integer, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& k, const ::jxx::Ptr<::jxx::lang::Integer>& v) {
    return ::jxx::lang::Integer::valueOf(1);
})));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(7), m->merge(::jxx::NEW<::jxx::lang::String>("c"), ::jxx::lang::Integer::valueOf(6), jxxMakeBiFunction<::jxx::lang::Integer, ::jxx::lang::Integer, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::Integer>& x, const ::jxx::Ptr<::jxx::lang::Integer>& y) {
    return ::jxx::lang::Integer::valueOf(((x)->intValue() + (y)->intValue()));
})));
        m->replaceAll(jxxMakeBiFunction<::jxx::lang::String, ::jxx::lang::Integer, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& k, const ::jxx::Ptr<::jxx::lang::Integer>& v) {
    return ::jxx::lang::Integer::valueOf(((v)->intValue() * 2));
}));
        EXPECT_EQ(::jxx::lang::Integer::valueOf(14), m->get(::jxx::NEW<::jxx::lang::String>("c")));
        const auto sum = ::jxx::NEW<::jxx::lang::IntArrayType>(1U);
        (*sum)[0U] = 0;
        m->forEach(jxxMakeBiConsumer<::jxx::lang::String, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& k, const ::jxx::Ptr<::jxx::lang::Integer>& v) {
    (*sum)[0] += (v)->intValue();
}));
        EXPECT_EQ(24, (*sum)[0]);
        EXPECT_TRUE(m->remove(::jxx::NEW<::jxx::lang::String>("a"), ::jxx::lang::Integer::valueOf(6)));
    }

} // namespace jxx::jdk8::util
