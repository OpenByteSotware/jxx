#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.TreeMap.h"
namespace { using S=::jxx::lang::String; using M=::jxx::util::TreeMap<S,S>; auto v(const char*x){return ::jxx::NEW<S>(x);} TEST(TreeMapIntegrationTest,OrderingAndMutation){auto m=::jxx::NEW<M>();m->put(v("b"),v("2"));m->put(v("a"),v("1"));m->put(v("c"),v("3"));EXPECT_EQ("a",m->firstKey()->utf8());EXPECT_EQ("c",m->lastKey()->utf8());EXPECT_EQ("2",m->put(v("b"),v("B"))->utf8());EXPECT_EQ("B",m->get(v("b"))->utf8());EXPECT_EQ("1",m->remove(v("a"))->utf8());EXPECT_EQ(2,m->size());} TEST(TreeMapIntegrationTest,RejectsNullKey){auto m=::jxx::NEW<M>();EXPECT_THROW(m->put(nullptr,v("x")),::jxx::lang::NullPointerException);} }
