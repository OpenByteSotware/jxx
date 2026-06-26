#include <memory>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>

#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.HashSet.h"

namespace {

using jxx::lang::String;
using jxx::util::HashSet;

static jxx::Ptr<String> S(const char* value) {
    return std::make_shared<String>(value);
}

TEST(HashSetJava8ParityTest, InheritanceShapeMatchesJava8Stack) {
    using HS = HashSet<String>;
    static_assert(std::is_base_of_v<jxx::util::AbstractSet<String>, HS>, "HashSet should extend AbstractSet<E>");
    static_assert(std::is_base_of_v<jxx::lang::Cloneable, HS>, "HashSet should implement Cloneable");
    static_assert(std::is_base_of_v<jxx::io::Serializable, HS>, "HashSet should implement Serializable");
    SUCCEED();
}

TEST(HashSetJava8ParityTest, BasicSetSemanticsWork) {
    auto set = std::make_shared<HashSet<String>>();
    EXPECT_TRUE(set->isEmpty());
    EXPECT_TRUE(set->add(S("alpha")));
    EXPECT_FALSE(set->add(S("alpha")));
    EXPECT_TRUE(set->contains(S("alpha")));
    EXPECT_EQ(set->size(), 1);
    EXPECT_TRUE(set->remove(S("alpha")));
    EXPECT_FALSE(set->remove(S("alpha")));
    EXPECT_TRUE(set->isEmpty());
}

TEST(HashSetJava8ParityTest, IteratorRemoveUpdatesSet) {
    auto set = std::make_shared<HashSet<String>>();
    set->add(S("a"));
    set->add(S("b"));

    auto it = set->iterator();
    ASSERT_TRUE(it->hasNext());
    auto first = it->next();
    ASSERT_NE(first, nullptr);
    it->remove();
    EXPECT_EQ(set->size(), 1);
}

TEST(HashSetJava8ParityTest, IteratorIsFailFast) {
    auto set = std::make_shared<HashSet<String>>();
    set->add(S("a"));
    set->add(S("b"));

    auto it = set->iterator();
    set->add(S("c"));
    EXPECT_THROW(it->next(), jxx::util::ConcurrentModificationException);
}

TEST(HashSetJava8ParityTest, SerializableContractMethodsExist) {
    auto set = std::make_shared<HashSet<String>>();
    EXPECT_THROW(set->writeObject(nullptr), jxx::lang::NullPointerException);
    EXPECT_THROW(set->readObject(nullptr), jxx::lang::NullPointerException);
    EXPECT_THROW(set->readObjectNoData(), jxx::lang::UnsupportedOperationException);
}

} // namespace
