#include <gtest/gtest.h>
#include <thread>
#include <type_traits>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Vector.h"

namespace {
using String = jxx::lang::String;
using VectorType = jxx::util::Vector<String>;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(VectorTest, CoreAndLegacyOperationsWork) {
    auto v = jxx::NEW<VectorType>();
    v->addElement(S("a"));
    v->insertElementAt(S("b"), 1);
    EXPECT_EQ(v->capacity(), 10);
    EXPECT_EQ(v->firstElement()->utf8(), "a");
    EXPECT_EQ(v->lastElement()->utf8(), "b");
    v->setElementAt(S("B"), 1);
    EXPECT_EQ(v->elementAt(1)->utf8(), "B");
    EXPECT_TRUE(v->removeElement(jxx::CAST<jxx::lang::Object>(S("a"))));
    EXPECT_EQ(v->size(), 1);
}

TEST(VectorTest, EnumerationAndListIteratorUseSharedOwner) {
    auto v = jxx::NEW<VectorType>();
    v->add(S("one")); v->add(S("two"));
    auto e = v->elements();
    EXPECT_EQ(e->nextElement()->utf8(), "one");
    EXPECT_EQ(e->nextElement()->utf8(), "two");
    EXPECT_THROW(e->nextElement(), jxx::util::NoSuchElementException);
    auto it = v->listIterator();
    EXPECT_EQ(it->next()->utf8(), "one");
    v->add(S("three"));
    EXPECT_THROW(it->next(), jxx::util::ConcurrentModificationException);
}

TEST(VectorTest, CloneIsShallowWithIndependentStorage) {
    auto v = jxx::NEW<VectorType>();
    auto shared = S("shared");
    v->add(shared);
    auto copy = jxx::CAST<VectorType>(v->clone());
    ASSERT_NE(copy, nullptr);
    EXPECT_EQ(copy->get(0).get(), shared.get());
    copy->add(S("other"));
    EXPECT_EQ(v->size(), 1);
}

TEST(VectorTest, SynchronizedMutationDoesNotLoseElements) {
    auto v = jxx::NEW<VectorType>();
    constexpr int count = 100;
    auto worker = [&]() { for (int i = 0; i < count; ++i) v->add(S("x")); };
    std::thread a(worker), b(worker);
    a.join(); b.join();
    EXPECT_EQ(v->size(), count * 2);
}
} // namespace
