#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Collections.h"
#include "util/jxx.util.List.h"
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
    TEST(ArraysTest, SortAndParallelSortOrderValues) {
        auto a = ::jxx::NEW<::jxx::lang::IntArrayType>(3U);
        (*a)[0U] = 3;
        (*a)[1U] = 1;
        (*a)[2U] = 2;
        ::jxx::util::Arrays::sort(a);
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
        auto b = ::jxx::NEW<::jxx::lang::IntArrayType>(5U);
        (*b)[0U] = 5;
        (*b)[1U] = 4;
        (*b)[2U] = 3;
        (*b)[3U] = 2;
        (*b)[4U] = 1;
        ::jxx::util::Arrays::parallelSort(b, 1, 4);
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), b));
        auto s = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(3U);
        (*s)[0U] = ::jxx::NEW<::jxx::lang::String>("b");
        (*s)[1U] = ::jxx::NEW<::jxx::lang::String>("a");
        (*s)[2U] = ::jxx::NEW<::jxx::lang::String>("c");
        ::jxx::util::Arrays::sort(s, ::jxx::util::Collections::reverseOrder<::jxx::lang::String>());
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(), s));
    }

    TEST(ArraysTest, BinarySearchFindsValuesAndInsertionPoints) {
        auto a = ::jxx::NEW<::jxx::lang::IntArrayType>(4U);
        (*a)[0U] = 1;
        (*a)[1U] = 3;
        (*a)[2U] = 5;
        (*a)[3U] = 7;
        EXPECT_EQ(2, ::jxx::util::Arrays::binarySearch(a, 5));
        EXPECT_EQ(-3, ::jxx::util::Arrays::binarySearch(a, 4));
        EXPECT_EQ(1, ::jxx::util::Arrays::binarySearch(a, 1, 3, 3));
    }

    TEST(ArraysTest, EqualityDeepEqualityAndHashingWork) {
        auto a = ::jxx::NEW<::jxx::lang::IntArrayType>(2U);
        (*a)[0U] = 1;
        (*a)[1U] = 2;
        auto b = ::jxx::NEW<::jxx::lang::IntArrayType>(2U);
        (*b)[0U] = 1;
        (*b)[1U] = 2;
        EXPECT_TRUE(::jxx::util::Arrays::equals(a, b));
        EXPECT_EQ(::jxx::util::Arrays::hashCode(a), ::jxx::util::Arrays::hashCode(b));
        auto x = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>(2U);
        (*x)[0U] = ::jxx::NEW<::jxx::lang::IntArrayType>();
        (*x)[1U] = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>();
        auto y = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>(2U);
        (*y)[0U] = ::jxx::NEW<::jxx::lang::IntArrayType>();
        (*y)[1U] = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>();
        EXPECT_TRUE(::jxx::util::Arrays::deepEquals(x, y));
        EXPECT_EQ(::jxx::util::Arrays::deepHashCode(x), ::jxx::util::Arrays::deepHashCode(y));
    }

    TEST(ArraysTest, FillCopyAndRangeCopyWork) {
        auto a = ::jxx::NEW<::jxx::lang::IntArrayType>(4);
        ::jxx::util::Arrays::fill(a, 7);
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
        ::jxx::util::Arrays::fill(a, 1, 3, 9);
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), ::jxx::util::Arrays::copyOf(a, 6)));
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), ::jxx::util::Arrays::copyOfRange(a, 1, 3)));
    }

    TEST(ArraysTest, ListAndStringRepresentationsWork) {
        auto a = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(2U);
        (*a)[0U] = ::jxx::NEW<::jxx::lang::String>("a");
        (*a)[1U] = ::jxx::NEW<::jxx::lang::String>("b");
        ::jxx::Ptr<::jxx::util::List<::jxx::lang::String>> list = ::jxx::util::Arrays::asList(a);
        list->set(0, ::jxx::NEW<::jxx::lang::String>("x"));
        EXPECT_EQ("x", (*a)[0]);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("[1, 2]"), ::jxx::lang::String::valueOf(::jxx::util::Arrays::toString(::jxx::NEW<::jxx::lang::IntArrayType>()))));
        EXPECT_EQ("[[1, 2]]", ::jxx::util::Arrays::deepToString(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>()));
    }

    TEST(ArraysTest, SetAllParallelSetAllAndPrefixWork) {
        auto a = ::jxx::NEW<::jxx::lang::IntArrayType>(4);
        ::jxx::util::Arrays::setAll(a, jxxMakeIntUnaryOperator([&](::jxx::lang::jint i) { return (i * 2); }));
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
        ::jxx::util::Arrays::parallelSetAll(a, jxxMakeIntUnaryOperator([&](::jxx::lang::jint i) { return (i + 1); }));
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
        ::jxx::util::Arrays::parallelPrefix(a, jxxMakeIntBinaryOperator([&](::jxx::lang::jint x, ::jxx::lang::jint y) { return (x + y); }));
        EXPECT_TRUE(::jxx::util::Arrays::equals(::jxx::NEW<::jxx::lang::IntArrayType>(), a));
    }

    TEST(ArraysTest, StreamsCoverFullAndPartialRanges) {
        EXPECT_EQ(6, ::jxx::util::Arrays::stream(::jxx::NEW<::jxx::lang::IntArrayType>())->sum());
        EXPECT_EQ(5, ::jxx::util::Arrays::stream(::jxx::NEW<::jxx::lang::IntArrayType>(), 1, 3)->sum());
        EXPECT_NE(::jxx::util::Arrays::spliterator(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>()), nullptr);
    }

} // namespace jxx::jdk8::util
