#include <gtest/gtest.h>
#include "util/jxx.util.OptionalInt.h"
#include "util/jxx.util.OptionalLong.h"
#include "util/jxx.util.OptionalDouble.h"
TEST(JxxOptionalPrimitives, EmptyAndPresent){auto e=::jxx::util::OptionalInt::empty();EXPECT_FALSE(e->isPresent());EXPECT_THROW(e->getAsInt(),::jxx::util::NoSuchElementException);auto v=::jxx::util::OptionalInt::of(7);EXPECT_TRUE(v->isPresent());EXPECT_EQ(v->getAsInt(),7);EXPECT_EQ(v->orElse(3),7);}
TEST(JxxOptionalPrimitives, ValueSemantics){auto a=::jxx::util::OptionalLong::of(9);auto b=::jxx::util::OptionalLong::of(9);EXPECT_TRUE(a->equals(b));EXPECT_EQ(a->hashCode(),b->hashCode());EXPECT_EQ(::jxx::util::OptionalDouble::empty()->orElse(2.5),2.5);}
