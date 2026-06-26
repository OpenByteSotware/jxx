#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Iterable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.Vector.h"

namespace {

using jxx::Ptr;
using jxx::lang::String;
using jxx::util::Vector;

static Ptr<String> S(const char* value) {
    return jxx::NEW<String>(value);
}

TEST(VectorTest, InheritanceMatchesJavaShape) {
    using V = Vector<String>;
    static_assert(std::is_base_of_v<jxx::util::AbstractList<String>, V>, "Vector should extend AbstractList<E>");
    static_assert(std::is_base_of_v<jxx::io::Serializable, V>, "Vector should implement Serializable");
    static_assert(std::is_base_of_v<jxx::lang::Cloneable, V>, "Vector should implement Cloneable");
    static_assert(std::is_base_of_v<jxx::lang::Iterable<String>, V>, "Vector should implement Iterable<E>");
    static_assert(std::is_base_of_v<jxx::util::Collection<String>, V>, "Vector should implement Collection<E>");
    static_assert(std::is_base_of_v<jxx::util::List<String>, V>, "Vector should implement List<E>");
    static_assert(std::is_base_of_v<jxx::util::RandomAccess, V>, "Vector should implement RandomAccess");
    SUCCEED();
}

TEST(VectorTest, CoreElementOperationsWork) {
    auto v = std::make_shared<Vector<String>>();
    EXPECT_TRUE(v->isEmpty());

    v->addElement(S("a"));
    v->addElement(S("b"));
    v->insertElementAt(S("x"), 1);

    EXPECT_EQ(v->size(), 3);
    EXPECT_EQ(v->capacity(), 10);
    ASSERT_NE(v->firstElement(), nullptr);
    EXPECT_EQ(v->firstElement()->utf8(), std::string("a"));
    ASSERT_NE(v->elementAt(1), nullptr);
    EXPECT_EQ(v->elementAt(1)->utf8(), std::string("x"));
    ASSERT_NE(v->lastElement(), nullptr);
    EXPECT_EQ(v->lastElement()->utf8(), std::string("b"));

    auto old = v->set(1, S("y"));
    ASSERT_NE(old, nullptr);
    EXPECT_EQ(old->utf8(), std::string("x"));
    EXPECT_EQ(v->elementAt(1)->utf8(), std::string("y"));

    auto removed = v->remove(1);
    ASSERT_NE(removed, nullptr);
    EXPECT_EQ(removed->utf8(), std::string("y"));
    EXPECT_EQ(v->size(), 2);
    EXPECT_EQ(v->toString()->utf8(), std::string("[a, b]"));
}

TEST(VectorTest, EnumerationTraversesOwnerStateSafely) {
    auto v = std::make_shared<Vector<String>>();
    v->addElement(S("one"));
    v->addElement(S("two"));
    v->addElement(S("three"));

    auto e = v->elements();
    ASSERT_NE(e, nullptr);
    ASSERT_TRUE(e->hasMoreElements());
    EXPECT_EQ(e->nextElement()->utf8(), std::string("one"));
    ASSERT_TRUE(e->hasMoreElements());
    EXPECT_EQ(e->nextElement()->utf8(), std::string("two"));
    ASSERT_TRUE(e->hasMoreElements());
    EXPECT_EQ(e->nextElement()->utf8(), std::string("three"));
    EXPECT_FALSE(e->hasMoreElements());
}

TEST(VectorTest, CloneEqualsAndHashCodeTrackContents) {
    auto v = std::make_shared<Vector<String>>();
    v->addElement(S("left"));
    v->addElement(S("right"));

    auto clonedObj = v->clone();
    auto cloned = jxx::CAST<Vector<String>, jxx::lang::Object>(clonedObj);
    ASSERT_NE(cloned, nullptr);
    EXPECT_TRUE(v->equals(clonedObj));
    EXPECT_EQ(v->hashCode(), cloned->hashCode());
    EXPECT_EQ(cloned->toString()->utf8(), std::string("[left, right]"));
}

TEST(VectorTest, SynchronizeHelperGuardsConcurrentMutation) {
    auto v = std::make_shared<Vector<String>>();
    constexpr int perThread = 300;

    auto worker = [&](const char* prefix) {
        for (int i = 0; i < perThread; ++i) {
            auto s = std::make_shared<String>(std::string(prefix) + std::to_string(i));
            v->addElement(s);
        }
    };

    std::thread t1(worker, "A");
    std::thread t2(worker, "B");
    t1.join();
    t2.join();

    EXPECT_EQ(v->size(), perThread * 2);
}

} // anonymous namespace
