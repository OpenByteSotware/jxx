#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "util/jxx.util.LinkedHashMap.h"
#include "util/jxx.util.Properties.h"
#include "util/jxx.util.TreeSet.h"
#include "util/jxx.util.WeakHashMap.h"

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeMaps() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Properties>();
        using ErasedHashMap = HashMap<::jxx::lang::Object, ::jxx::lang::Object>;
        registerType<ErasedHashMap>();
        ::jxx::io::serialization::registerSerializableClass<ErasedHashMap>(
                    static_cast<::jxx::lang::jlong>(362498820763181265LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedHashMap, 1U, 2U>();
        using ErasedIdentityHashMap = IdentityHashMap<::jxx::lang::Object, ::jxx::lang::Object>;
        registerType<ErasedIdentityHashMap>();
        ::jxx::io::serialization::registerSerializableClass<ErasedIdentityHashMap>(static_cast<::jxx::lang::jlong>(8188218128353913216LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedIdentityHashMap, 1U, 2U>();
        using ErasedLinkedHashMap = LinkedHashMap<::jxx::lang::Object, ::jxx::lang::Object>;
        registerType<ErasedLinkedHashMap>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedHashMap>(static_cast<::jxx::lang::jlong>(3801124242820219131LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedHashMap, 1U, 2U>();
        using ErasedWeakHashMap = WeakHashMap<::jxx::lang::Object, ::jxx::lang::Object>;
        registerType<ErasedWeakHashMap>();
        using ErasedTreeSet = TreeSet<::jxx::lang::Object>;
        registerType<ErasedTreeSet>();
        ::jxx::io::serialization::registerSerializableClass<ErasedTreeSet>(static_cast<::jxx::lang::jlong>(-2479143000061671589LL), [] { return ::jxx::NEW<ErasedTreeSet>(); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedTreeSet, 1U, 2U>();
    });
}
}
