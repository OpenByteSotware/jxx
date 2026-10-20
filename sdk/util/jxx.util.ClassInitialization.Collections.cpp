#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.ArrayDeque.h"
#include "util/jxx.util.ArrayList.h"
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
        registerType<Arrays>();
        registerType<ArrayDeque<::jxx::lang::Object>>();
        ::jxx::io::serialization::registerSerializableClass<
                    ArrayDeque<::jxx::lang::Object>>(
                        static_cast<::jxx::lang::jlong>(2340985798034038923LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                    ArrayDeque<::jxx::lang::Object>, 1U, 2U>();
        registerType<ArrayList<::jxx::lang::Object>>();
        ::jxx::io::serialization::registerSerializableClass<
                    ArrayList<::jxx::lang::Object>>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                    ArrayList<::jxx::lang::Object>, 1U, 2U>();
        registerType<BitSet>();
        ::jxx::io::serialization::registerSerializableClass<BitSet>(1);
        registerType<Collections>();
        registerType<RandomAccess>();
        registerType<Spliterators>();
        registerType<::jxx::util::wildcard::CollectionAny>();
    });
}
}
