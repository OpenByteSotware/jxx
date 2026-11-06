#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.Optional.h"

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
    TEST(OptionalTest, CreationPresenceAndAccessWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> e = ::jxx::util::Optional<::jxx::lang::String>::empty();
        EXPECT_FALSE(e->isPresent());
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> p = ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x"));
        EXPECT_TRUE(p->isPresent());
        EXPECT_EQ("x", p->get());
        EXPECT_FALSE(::jxx::util::Optional<::jxx::lang::Object>::ofNullable(::jxx::Ptr<::jxx::lang::Object>{})->isPresent());
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional<::jxx::lang::String>::ofNullable(::jxx::NEW<::jxx::lang::String>("x")));
    }

    TEST(OptionalTest, OfRejectsNull) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional<::jxx::lang::Object>::of(::jxx::Ptr<::jxx::lang::Object>{});
            },
            ::jxx::lang::NullPointerException);
    }

    TEST(OptionalTest, GetRejectsEmpty) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional<::jxx::lang::Object>::empty()->get();
            },
            ::jxx::util::NoSuchElementException);
    }

    TEST(OptionalTest, ConditionalAndTransformingOperationsWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> p = ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("abc"));
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("abc")), p->filter(jxxMakePredicate<::jxx::lang::String>([&](const ::jxx::Ptr<::jxx::lang::String>& v) {
    return (v->length() == 3);
})));
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::Object>::empty(), p->filter(jxxMakePredicate<::jxx::lang::String>([&](const ::jxx::Ptr<::jxx::lang::String>& v) {
    return false;
})));
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::Integer>::of(::jxx::lang::Integer::valueOf(3)), p->map(jxxMakeFunction<::jxx::lang::String, ::jxx::lang::Integer>([&](const ::jxx::Ptr<::jxx::lang::String>& value) { return ::jxx::lang::Integer::valueOf(value->length()); })));
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("ABC")), p->flatMap(jxxMakeFunction<::jxx::lang::String, ::jxx::util::Optional<::jxx::lang::String>>([&](const ::jxx::Ptr<::jxx::lang::String>& v) {
    return ::jxx::util::Optional<::jxx::lang::Object>::of(v->toUpperCase());
})));
        ::jxx::Ptr<::jxx::util::concurrent::atomic::AtomicBoolean> called = ::jxx::NEW<::jxx::util::concurrent::atomic::AtomicBoolean>(false);
        p->ifPresent(jxxMakeConsumer<::jxx::lang::String>([&](const ::jxx::Ptr<::jxx::lang::String>& v) {
    called->set(true);
}));
        EXPECT_TRUE(called->get());
    }

    TEST(OptionalTest, FallbackOperationsWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> e = ::jxx::util::Optional<::jxx::lang::String>::empty();
        EXPECT_EQ("d", e->orElse(::jxx::NEW<::jxx::lang::String>("d")));
        EXPECT_EQ("s", e->orElseGet(jxxMakeSupplier<::jxx::lang::String>([&]() {
    return ::jxx::NEW<::jxx::lang::String>("s");
})));
        EXPECT_EQ("x", ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x"))->orElse(::jxx::NEW<::jxx::lang::String>("d")));
    }

    TEST(OptionalTest, OrElseThrowUsesSupplier) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional<::jxx::lang::Object>::empty()->orElseThrow(jxxMakeSupplier<::jxx::lang::IllegalStateException>([&]() {
    return ::jxx::NEW<::jxx::lang::IllegalStateException>(::jxx::NEW<::jxx::lang::String>("missing"));
}));
            },
            ::jxx::lang::IllegalStateException);
    }

    TEST(OptionalTest, EqualityHashAndStringWork) {
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::Object>::empty(), ::jxx::util::Optional<::jxx::lang::Object>::empty());
        EXPECT_EQ(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x")));
        EXPECT_NE(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("y")));
        EXPECT_EQ("x", ::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x"))->hashCode());
        EXPECT_EQ(0, ::jxx::util::Optional<::jxx::lang::Object>::empty()->hashCode());
        EXPECT_TRUE(::jxx::util::Optional<::jxx::lang::String>::of(::jxx::NEW<::jxx::lang::String>("x"))->toString()->contains(::jxx::NEW<::jxx::lang::String>("x")));
    }

} // namespace jxx::jdk8::util
