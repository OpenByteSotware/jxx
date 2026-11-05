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
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Objects.h"

namespace {
template<typename T, typename F> class JxxLambdaConsumer final : public ::jxx::lang::ClassBase<JxxLambdaConsumer<T,F>, ::jxx::lang::Object, ::jxx::util::function::Consumer<T>> { public: explicit JxxLambdaConsumer(F f):f_(std::move(f)){} void accept(const ::jxx::Ptr<T>& v) override { f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakeConsumer(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Consumer<T>>(::jxx::NEW<JxxLambdaConsumer<T,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaPredicate final : public ::jxx::lang::ClassBase<JxxLambdaPredicate<T,F>, ::jxx::lang::Object, ::jxx::util::function::Predicate<T>> { public: explicit JxxLambdaPredicate(F f):f_(std::move(f)){} ::jxx::lang::jbool test(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakePredicate(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Predicate<T>>(::jxx::NEW<JxxLambdaPredicate<T,D>>(std::forward<F>(f))); }
template<typename T, typename R, typename F> class JxxLambdaFunction final : public ::jxx::lang::ClassBase<JxxLambdaFunction<T,R,F>, ::jxx::lang::Object, ::jxx::util::function::Function<T,R>> { public: explicit JxxLambdaFunction(F f):f_(std::move(f)){} ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename R, typename F> auto jxxMakeFunction(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Function<T,R>>(::jxx::NEW<JxxLambdaFunction<T,R,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaUnaryOperator final : public ::jxx::lang::ClassBase<JxxLambdaUnaryOperator<T,F>, ::jxx::lang::Object, ::jxx::util::function::UnaryOperator<T>> { public: explicit JxxLambdaUnaryOperator(F f):f_(std::move(f)){} ::jxx::Ptr<T> apply(const ::jxx::Ptr<T>& v) override { return f_(v); } private: F f_; };
template<typename T, typename F> auto jxxMakeUnaryOperator(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::UnaryOperator<T>>(::jxx::NEW<JxxLambdaUnaryOperator<T,D>>(std::forward<F>(f))); }
template<typename T, typename F> class JxxLambdaSupplier final : public ::jxx::lang::ClassBase<JxxLambdaSupplier<T,F>, ::jxx::lang::Object, ::jxx::util::function::Supplier<T>> { public: explicit JxxLambdaSupplier(F f):f_(std::move(f)){} ::jxx::Ptr<T> get() override { return f_(); } private: F f_; };
template<typename T, typename F> auto jxxMakeSupplier(F&& f) { using D=std::decay_t<F>; return ::jxx::CAST<::jxx::util::function::Supplier<T>>(::jxx::NEW<JxxLambdaSupplier<T,D>>(std::forward<F>(f))); }
} // namespace

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
            sum += (n)->intValue();
        }
        EXPECT_EQ(6, sum);
        EXPECT_EQ(3, s->toArray()->length);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Integer>, 1U>> a = s->toArray(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Integer>, 1U>>(0));
        EXPECT_EQ(3, a->length);
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> c = ::jxx::CAST<::jxx::util::HashSet<::jxx::lang::Integer>>(s->clone());
        EXPECT_TRUE(::jxx::util::Objects::equals(s, c));
        EXPECT_NE(s.get(), c.get());
        EXPECT_NE(s->spliterator(), nullptr);
        s->clear();
        EXPECT_TRUE(s->isEmpty());
    }

    TEST(HashSetTest, RemoveIfAndForEachWork) {
        ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Integer>> s = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>(::jxx::util::Arrays::asList(1, 2, 3, 4));
        EXPECT_TRUE(s->removeIf(jxxMakePredicate<::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::Integer>& n) {
    return (((n)->intValue() % 2) == 0);
})));
        const auto sum = ::jxx::NEW<::jxx::lang::IntArrayType>(1U);
        (*sum)[0U] = 0;
        s->forEach(jxxMakeConsumer<::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::Integer>& n) {
    (*sum)[0] += (n)->intValue();
}));
        EXPECT_EQ(4, (*sum)[0]);
    }

} // namespace jxx::jdk8::util
