#include <gtest/gtest.h>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.LinkedHashSet.h"
#include "util/jxx.util.TreeMap.h"

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

TEST(UtilConcreteMapsSetsInitialization, RegistersErasedPublicTypes) {
    ::jxx::util::initializeClasses();

    using Object = ::jxx::lang::Object;
    using HashSet = ::jxx::util::HashSet<Object>;
    using LinkedHashSet = ::jxx::util::LinkedHashSet<Object>;
    using TreeMap = ::jxx::util::TreeMap<Object, Object>;

    expectSerializableRegistration<HashSet>(
        static_cast<::jxx::lang::jlong>(-5024744406713321676LL));
    expectSerializableRegistration<LinkedHashSet>(
        static_cast<::jxx::lang::jlong>(-2851667679971038690LL));
    expectSerializableRegistration<TreeMap>(
        static_cast<::jxx::lang::jlong>(919286545866124006LL));
}
