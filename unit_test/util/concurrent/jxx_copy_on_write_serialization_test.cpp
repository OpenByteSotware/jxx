#include <gtest/gtest.h>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArrayList.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArraySet.h"

namespace {
template <typename T>
void expectSerializableRegistration(::jxx::lang::jlong expectedUid) {
    const auto clazz = T::Class();
    ASSERT_NE(nullptr, clazz);
    ASSERT_NE(nullptr, clazz->getName());
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            clazz->getName()->utf8());
    EXPECT_EQ(expectedUid, metadata.serialVersionUID);
    ASSERT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    EXPECT_NE(nullptr, metadata.deserializationFactory());
}
} // namespace

TEST(CopyOnWriteSerialization, RegistersListAndSet) {
    ::jxx::util::concurrent::initializeClasses();
    using Object = ::jxx::lang::Object;
    expectSerializableRegistration<
        ::jxx::util::concurrent::CopyOnWriteArrayList<Object>>(
            static_cast<::jxx::lang::jlong>(8673264195747942595LL));
    expectSerializableRegistration<
        ::jxx::util::concurrent::CopyOnWriteArraySet<Object>>(
            static_cast<::jxx::lang::jlong>(5457747651344034263LL));
}

TEST(CopyOnWriteSerialization, FactoriesCreateEmptyCollections) {
    ::jxx::util::concurrent::initializeClasses();
    using Object = ::jxx::lang::Object;
    using List = ::jxx::util::concurrent::CopyOnWriteArrayList<Object>;
    using Set = ::jxx::util::concurrent::CopyOnWriteArraySet<Object>;

    const auto listMetadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            List::Class()->getName()->utf8());
    const auto setMetadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            Set::Class()->getName()->utf8());
    const auto list = ::jxx::CAST<List>(listMetadata.deserializationFactory());
    const auto set = ::jxx::CAST<Set>(setMetadata.deserializationFactory());
    ASSERT_NE(nullptr, list);
    ASSERT_NE(nullptr, set);
    EXPECT_TRUE(list->isEmpty());
    EXPECT_TRUE(set->isEmpty());
}
