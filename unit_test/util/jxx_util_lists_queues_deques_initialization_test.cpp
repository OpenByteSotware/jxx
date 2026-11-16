#include <gtest/gtest.h>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ArrayDeque.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.LinkedList.h"
#include "util/jxx.util.PriorityQueue.h"
#include "util/jxx.util.Vector.h"

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

TEST(UtilListsQueuesDequesInitialization, RegistersSerializableErasedTypes) {
    ::jxx::util::initializeClasses();
    using Object = ::jxx::lang::Object;

    expectSerializableRegistration<::jxx::util::ArrayList<Object>>(
        static_cast<::jxx::lang::jlong>(8683452581122892189LL));
    expectSerializableRegistration<::jxx::util::LinkedList<Object>>(
        static_cast<::jxx::lang::jlong>(876323262645176354LL));
    expectSerializableRegistration<::jxx::util::ArrayDeque<Object>>(
        static_cast<::jxx::lang::jlong>(2340985798034038923LL));
    expectSerializableRegistration<::jxx::util::PriorityQueue<Object>>(
        static_cast<::jxx::lang::jlong>(-7720805057305804111LL));
    expectSerializableRegistration<::jxx::util::Vector<Object>>(
        static_cast<::jxx::lang::jlong>(-2767605614048989439LL));
}
