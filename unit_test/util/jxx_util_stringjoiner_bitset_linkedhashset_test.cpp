#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.BitSet.h"
#include "util/jxx.util.LinkedHashSet.h"
#include "util/jxx.util.StringJoiner.h"
namespace {
using S=::jxx::lang::String;
::jxx::Ptr<::jxx::lang::CharSequence> cs(const char* v){return ::jxx::CAST<::jxx::lang::CharSequence>(::jxx::NEW<S>(v));}
TEST(StringJoinerParityTest, SupportsEmptyAddMergeAndLength){auto j=::jxx::NEW<::jxx::util::StringJoiner>(cs(","),cs("["),cs("]"));EXPECT_TRUE(j->toString()->utf8()=="[]");j->setEmptyValue(cs("empty"));EXPECT_TRUE(j->toString()->utf8()=="empty");j->add(cs("a"))->add(nullptr);auto other=::jxx::NEW<::jxx::util::StringJoiner>(cs("-"));other->add(cs("b"))->add(cs("c"));j->merge(other);EXPECT_TRUE(j->toString()->utf8()=="[a,null,b-c]");EXPECT_EQ(j->toString()->length(),j->length());}
TEST(BitSetParityTest, SupportsRangesSearchAndLogicalOperations){auto bits=::jxx::NEW<::jxx::util::BitSet>();bits->set(1);bits->set(64);bits->set(3,6);EXPECT_EQ(5,bits->cardinality());EXPECT_EQ(1,bits->nextSetBit(0));EXPECT_EQ(2,bits->nextClearBit(1));EXPECT_EQ(65,bits->length());auto mask=::jxx::NEW<::jxx::util::BitSet>();mask->set(4);mask->set(64);EXPECT_TRUE(bits->intersects(mask));bits->andSet(mask);EXPECT_EQ(2,bits->cardinality());EXPECT_TRUE(bits->toString()->utf8()=="{4, 64}");}
TEST(LinkedHashSetParityTest, PreservesFirstInsertionOrderAndUniqueness){auto set=::jxx::NEW<::jxx::util::LinkedHashSet<S>>();EXPECT_TRUE(set->add(::jxx::NEW<S>("b")));EXPECT_TRUE(set->add(::jxx::NEW<S>("a")));EXPECT_FALSE(set->add(::jxx::NEW<S>("b")));auto iterator=set->iterator();EXPECT_TRUE(iterator->next()->utf8()=="b");EXPECT_TRUE(iterator->next()->utf8()=="a");EXPECT_EQ(2,set->size());}
}
