#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.LinkedList.h"
namespace {
using String = ::jxx::lang::String;
using List = ::jxx::util::LinkedList<String>;
auto text(const char* value){return ::jxx::NEW<String>(value);}
TEST(LinkedListIntegrationTest, IndexedDequeAndIteratorMutationsStayConsistent) {
 const auto list=::jxx::NEW<List>(); const auto a=text("a"),b=text("b"),c=text("c"),x=text("x");
 list->addLast(a); list->addLast(c); list->add(1,b); list->addFirst(x);
 ASSERT_EQ(4,list->size()); EXPECT_TRUE(list->get(0)->equals(x)); EXPECT_TRUE(list->get(2)->equals(b));
 EXPECT_TRUE(list->set(2,c)->equals(b)); EXPECT_TRUE(list->remove(1)->equals(a)); EXPECT_TRUE(list->removeFirst()->equals(x)); EXPECT_TRUE(list->removeLast()->equals(c)); ASSERT_EQ(1,list->size()); EXPECT_TRUE(list->getFirst()->equals(c)); EXPECT_TRUE(list->removeFirst()->equals(c)); EXPECT_TRUE(list->isEmpty());
}
TEST(LinkedListIntegrationTest, ListIteratorSupportsBidirectionalMutation) {
 const auto list=::jxx::NEW<List>(); list->add(text("a")); list->add(text("c"));
 const auto it=list->listIterator(1); it->add(text("b")); ASSERT_TRUE(it->hasPrevious()); EXPECT_TRUE(it->previous()->equals(text("b"))); it->set(text("B"));
 ASSERT_EQ(3,list->size()); EXPECT_TRUE(list->get(1)->equals(text("B")));
}
TEST(LinkedListIntegrationTest, IteratorIsFailFastAfterExternalMutation) {
 const auto list=::jxx::NEW<List>(); list->add(text("a")); const auto it=list->iterator(); list->add(text("b")); EXPECT_THROW((void)it->next(),::jxx::util::ConcurrentModificationException);
}
}
