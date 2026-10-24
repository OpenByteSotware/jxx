#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.Base64.h"
#include "util/jxx.util.DoubleSummaryStatistics.h"
#include "util/jxx.util.EventListener.h"
#include "util/jxx.util.EventObject.h"
#include "util/jxx.util.Formattable.h"
#include "util/jxx.util.Formatter.h"
#include "util/jxx.util.IntStream.h"
#include "util/jxx.util.IntSummaryStatistics.h"
#include "util/jxx.util.LinkedHashSet.h"
#include "util/jxx.util.LinkedList.h"
#include "util/jxx.util.LongSummaryStatistics.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.Observable.h"
#include "util/jxx.util.Observer.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"
#include "util/jxx.util.OptionalLong.h"
#include "util/jxx.util.Random.h"
#include "util/jxx.util.Scanner.h"
#include "util/jxx.util.SplittableRandom.h"
#include "util/jxx.util.StringJoiner.h"
#include "util/jxx.util.StringTokenizer.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"
#include "util/jxx.util.UUID.h"

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeUtilities() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Base64>();
        registerType<DoubleSummaryStatistics>();
        registerType<EventListener>();
        registerType<EventObject>();
        ::jxx::io::serialization::registerSerializableClass<EventObject>(1, [] { return ::jxx::NEW<EventObject>(::jxx::NEW<::jxx::lang::Object>()); });
        registerType<Formattable>();
        registerType<Formatter>();
        using ErasedLinkedList = LinkedList<::jxx::lang::Object>;
        registerType<ErasedLinkedList>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedList>(
                    static_cast<::jxx::lang::jlong>(876323262645176354LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedList, 1U, 2U>();
        using ErasedLinkedHashSet = LinkedHashSet<::jxx::lang::Object>;
        registerType<ErasedLinkedHashSet>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedHashSet>(
                    static_cast<::jxx::lang::jlong>(-2851667679971038690LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedHashSet, 1U, 2U>();
        registerType<IntStream>();
        registerType<IntSummaryStatistics>();
        registerType<LongSummaryStatistics>();
        registerType<Objects>();
        registerType<Observable>();
        registerType<Observer>();
        registerType<OptionalDouble>();
        registerType<OptionalInt>();
        registerType<OptionalLong>();
        registerType<Random>();
        ::jxx::io::serialization::registerSerializableClass<Random>(1);
        registerType<Scanner>();
        registerType<SplittableRandom>();
        registerType<StringJoiner>();
        registerType<StringTokenizer>();
        registerType<Timer>();
        registerType<TimerTask>();
        registerType<UUID>();
        ::jxx::io::serialization::registerSerializableClass<UUID>(1, [] {
                    return ::jxx::NEW<UUID>(0, 0);
                });
    });
}
}
