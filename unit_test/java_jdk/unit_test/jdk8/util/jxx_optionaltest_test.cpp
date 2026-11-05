#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.Optional.h"

namespace jxx::jdk8::util {
    TEST(OptionalTest, CreationPresenceAndAccessWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> e = ::jxx::util::Optional::empty();
        EXPECT_FALSE(e->isPresent());
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> p = ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x"));
        EXPECT_TRUE(p->isPresent());
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("x"), p->get()));
        EXPECT_FALSE(::jxx::util::Optional::ofNullable(nullptr)->isPresent());
        EXPECT_EQ(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional::ofNullable(::jxx::NEW<::jxx::lang::String>("x")));
    }

    TEST(OptionalTest, OfRejectsNull) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional::of(nullptr);
            },
            ::jxx::lang::NullPointerException);
    }

    TEST(OptionalTest, GetRejectsEmpty) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional::empty()->get();
            },
            ::jxx::util::NoSuchElementException);
    }

    TEST(OptionalTest, ConditionalAndTransformingOperationsWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> p = ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("abc"));
        EXPECT_EQ(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("abc")), p->filter([&](auto v) {
    (v->length() == 3);
}));
        EXPECT_EQ(::jxx::util::Optional::empty(), p->filter([&](auto v) {
    false;
}));
        EXPECT_EQ(::jxx::util::Optional::of(3), p->map(MethodReference(expression=MemberReference(member=String, postfix_operators=[], prefix_operators=[], qualifier=, selectors=[]), method=MemberReference(member=length, postfix_operators=[], prefix_operators=[], qualifier=, selectors=[]), type_arguments=[])));
        EXPECT_EQ(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("ABC")), p->flatMap([&](auto v) {
    ::jxx::util::Optional::of(v->toUpperCase());
}));
        ::jxx::Ptr<::jxx::util::concurrent::atomic::AtomicBoolean> called = ::jxx::NEW<::jxx::util::concurrent::atomic::AtomicBoolean>(false);
        p->ifPresent([&](auto v) {
    called->set(true);
});
        EXPECT_TRUE(called->get());
    }

    TEST(OptionalTest, FallbackOperationsWork) {
        ::jxx::Ptr<::jxx::util::Optional<::jxx::lang::String>> e = ::jxx::util::Optional::empty();
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("d"), e->orElse(::jxx::NEW<::jxx::lang::String>("d"))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("s"), e->orElseGet([&]() {
    "s";
})));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("x"), ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x"))->orElse(::jxx::NEW<::jxx::lang::String>("d"))));
    }

    TEST(OptionalTest, OrElseThrowUsesSupplier) {
        EXPECT_THROW(
            {
                ::jxx::util::Optional::empty()->orElseThrow([&]() {
    ::jxx::NEW<::jxx::lang::IllegalStateException>(::jxx::NEW<::jxx::lang::String>("missing"));
});
            },
            ::jxx::lang::IllegalStateException);
    }

    TEST(OptionalTest, EqualityHashAndStringWork) {
        EXPECT_EQ(::jxx::util::Optional::empty(), ::jxx::util::Optional::empty());
        EXPECT_EQ(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x")));
        EXPECT_NE(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x")), ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("y")));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("x"), ::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x"))->hashCode()));
        EXPECT_EQ(0, ::jxx::util::Optional::empty()->hashCode());
        EXPECT_TRUE(::jxx::util::Optional::of(::jxx::NEW<::jxx::lang::String>("x"))->toString()->contains(::jxx::NEW<::jxx::lang::String>("x")));
    }

} // namespace jxx::jdk8::util
