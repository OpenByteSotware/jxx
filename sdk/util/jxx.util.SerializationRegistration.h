#pragma once

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/jxx.util.Vector.h"

namespace jxx::util::serialization {

inline constexpr ::jxx::lang::jlong VECTOR_SERIAL_VERSION_UID =
    static_cast<::jxx::lang::jlong>(-2767605614048989439LL);
inline constexpr ::jxx::lang::jlong CONCURRENT_HASH_MAP_SERIAL_VERSION_UID =
    static_cast<::jxx::lang::jlong>(7249069246763182397LL);

template <typename E>
void registerVector()
{
    using Type = ::jxx::util::Vector<E>;
    (void)Type::Class();
    ::jxx::io::serialization::registerSerializableClass<Type>(
        VECTOR_SERIAL_VERSION_UID);
}

template <typename K, typename V>
void registerConcurrentHashMap()
{
    using Type = ::jxx::util::concurrent::ConcurrentHashMap<K, V>;
    (void)Type::Class();
    ::jxx::io::serialization::registerSerializableClass<Type>(
        CONCURRENT_HASH_MAP_SERIAL_VERSION_UID);
}

// Raw source collections are represented by one erased Object specialization.
// Registering multiple Vector<T> factories is invalid because every
// specialization has the same source binary class name and registry key.
inline void registerRawVectorCacheGraph()
{
    using RawVector = ::jxx::util::Vector<::jxx::lang::Object>;
    using CacheMap = ::jxx::util::concurrent::ConcurrentHashMap<
        ::jxx::lang::Integer, RawVector>;

    registerVector<::jxx::lang::Object>();
    registerConcurrentHashMap<::jxx::lang::Integer, RawVector>();
    (void)CacheMap::Class();
}

} // namespace jxx::util::serialization
