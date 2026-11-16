#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.AbstractSequentialList.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.NavigableSet.h"
#include "util/jxx.util.Queue.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.SortedMap.h"
#include "util/jxx.util.SortedSet.h"

TEST(UtilCoreCollectionClassInitialization, RegistersPublicParityTypes) {
    ::jxx::util::initializeClasses();

    using Object = ::jxx::lang::Object;
    EXPECT_NE(nullptr, ::jxx::util::AbstractCollection<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::AbstractList<Object>::Class());
    EXPECT_NE(nullptr, (::jxx::util::AbstractMap<Object, Object>::Class()));
    EXPECT_NE(nullptr, ::jxx::util::AbstractQueue<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::AbstractSequentialList<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::AbstractSet<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::Collection<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::Deque<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::List<Object>::Class());
    EXPECT_NE(nullptr, (::jxx::util::Map<Object, Object>::Class()));
    EXPECT_NE(nullptr, (::jxx::util::NavigableMap<Object, Object>::Class()));
    EXPECT_NE(nullptr, ::jxx::util::NavigableSet<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::Queue<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::Set<Object>::Class());
    EXPECT_NE(nullptr, (::jxx::util::SortedMap<Object, Object>::Class()));
    EXPECT_NE(nullptr, ::jxx::util::SortedSet<Object>::Class());
}
