#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.ArrayDeque.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.AbstractSequentialList.h"
#include "util/jxx.util.AbstractSet.h"
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
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.PriorityQueue.h"
#include "util/jxx.util.Vector.h"
#include "util/jxx.util.Stack.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.BitSet.h"
#include "util/jxx.util.Collections.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.Spliterators.h"
#include "util/jxx.util.wildcard.CollectionAny.h"

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeCollections() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        // Register the erased Object specializations used to represent the
        // public generic collection API in the runtime class metadata system.
        registerType<AbstractCollection<::jxx::lang::Object>>();
        registerType<AbstractList<::jxx::lang::Object>>();
        registerType<AbstractMap<::jxx::lang::Object, ::jxx::lang::Object>>();
        registerType<AbstractQueue<::jxx::lang::Object>>();
        registerType<AbstractSequentialList<::jxx::lang::Object>>();
        registerType<AbstractSet<::jxx::lang::Object>>();
        registerType<Collection<::jxx::lang::Object>>();
        registerType<Deque<::jxx::lang::Object>>();
        registerType<List<::jxx::lang::Object>>();
        registerType<Map<::jxx::lang::Object, ::jxx::lang::Object>>();
        registerType<NavigableMap<::jxx::lang::Object, ::jxx::lang::Object>>();
        registerType<NavigableSet<::jxx::lang::Object>>();
        registerType<Queue<::jxx::lang::Object>>();
        registerType<Set<::jxx::lang::Object>>();
        registerType<SortedMap<::jxx::lang::Object, ::jxx::lang::Object>>();
        registerType<SortedSet<::jxx::lang::Object>>();
        registerType<Arrays>();
        registerType<ArrayDeque<::jxx::lang::Object>>();
        ::jxx::io::serialization::registerSerializableClass<
                    ArrayDeque<::jxx::lang::Object>>(
                        static_cast<::jxx::lang::jlong>(2340985798034038923LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                    ArrayDeque<::jxx::lang::Object>, 1U, 2U>();
        registerType<ArrayList<::jxx::lang::Object>>();
        ::jxx::io::serialization::registerSerializableClass<
                    ArrayList<::jxx::lang::Object>>(
                        static_cast<::jxx::lang::jlong>(8683452581122892189LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                    ArrayList<::jxx::lang::Object>, 1U, 2U>();
        using ErasedPriorityQueue = PriorityQueue<::jxx::lang::Object>;
        registerType<ErasedPriorityQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedPriorityQueue>(
            static_cast<::jxx::lang::jlong>(-7720805057305804111LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedPriorityQueue, 1U, 2U>();

        using ErasedVector = Vector<::jxx::lang::Object>;
        registerType<ErasedVector>();
        ::jxx::io::serialization::registerSerializableClass<ErasedVector>(
            static_cast<::jxx::lang::jlong>(-2767605614048989439LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedVector, 1U, 2U>();

        registerType<BitSet>();
        ::jxx::io::serialization::registerSerializableClass<BitSet>(static_cast<::jxx::lang::jlong>(7997698588986878753LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<BitSet, 1U, 2U>();
        registerType<Collections>();
        registerType<RandomAccess>();
        registerType<Spliterators>();
        registerType<::jxx::util::wildcard::CollectionAny>();
    });
}
}
