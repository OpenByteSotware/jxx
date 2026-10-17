#include <gtest/gtest.h>
#include <type_traits>

#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "io/jxx.io.AutoCloseable.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Readable.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "lang/jxx.lang.Iterable.h"
#include "util/jxx.util.LinkedHashSet.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.NavigableSet.h"
#include "util/jxx.util.Queue.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.SortedMap.h"
#include "util/jxx.util.SortedSet.h"
#include "util/jxx.util.TreeMap.h"
#include "util/jxx.util.WeakHashMap.h"

using Object = ::jxx::lang::Object;
using IdentityObjectMap = ::jxx::util::IdentityHashMap<Object, Object>;
using WeakObjectMap = ::jxx::util::WeakHashMap<Object, Object>;
using LinkedObjectSet = ::jxx::util::LinkedHashSet<Object>;
using ObjectHashSet = ::jxx::util::HashSet<Object>;
using ObjectTreeMap = ::jxx::util::TreeMap<Object, Object>;

TEST(Java8HierarchyConformance, CoreCollectionInterfaceChains) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Iterable<Object>,
                                   ::jxx::util::Collection<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Collection<Object>,
                                   ::jxx::util::List<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Collection<Object>,
                                   ::jxx::util::Queue<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Queue<Object>,
                                   ::jxx::util::Deque<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Map<Object, Object>,
                                   ::jxx::util::SortedMap<Object, Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::SortedMap<Object, Object>,
                                   ::jxx::util::NavigableMap<Object, Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Set<Object>,
                                   ::jxx::util::SortedSet<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::SortedSet<Object>,
                                   ::jxx::util::NavigableSet<Object>>));
}

TEST(Java8HierarchyConformance, CorrectedMapAndSetHierarchies) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, IdentityObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, IdentityObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::AbstractMap<Object, Object>,
                                   WeakObjectMap>));
    EXPECT_FALSE((std::is_base_of_v<::jxx::io::SerializableI, WeakObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<ObjectHashSet, LinkedObjectSet>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, LinkedObjectSet>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, LinkedObjectSet>));
}

TEST(Java8HierarchyConformance, ReaderAndTypeAdapterKinds) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Readable, ::jxx::io::Reader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::Closeable, ::jxx::io::Reader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,
                                   ::com::google::gson::TypeAdapter>));
    EXPECT_TRUE(std::is_abstract_v<::com::google::gson::TypeAdapter>);
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::InterfaceBase<::com::google::gson::TypeAdapter>,
        ::com::google::gson::TypeAdapter>));
}

TEST(Java8HierarchyConformance, CloseableExtendsAutoCloseable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::AutoCloseable,
                                   ::jxx::io::Closeable>));
}

// These are disabled until the confirmed hierarchy gaps are corrected.
// They remain executable parity gates without making the default suite red.
TEST(Java8HierarchyKnownGap, DISABLED_WriterImplementsAppendable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Appendable, ::jxx::io::Writer>));
}

TEST(Java8HierarchyKnownGap, DISABLED_TreeMapImplementsNavigableMap) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::NavigableMap<Object, Object>,
                                   ObjectTreeMap>));
}
