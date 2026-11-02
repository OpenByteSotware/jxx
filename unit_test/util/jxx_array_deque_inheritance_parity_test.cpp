#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.ArrayDeque.h"
#include "util/jxx.util.Deque.h"

namespace {
using StringDeque = ::jxx::util::ArrayDeque<::jxx::lang::String>;
}

TEST(ArrayDequeInheritanceParity, UsesSourceSuperclass) {
    static_assert(std::is_base_of_v<
        ::jxx::util::AbstractCollection<::jxx::lang::String>, StringDeque>);
    static_assert(!std::is_base_of_v<
        ::jxx::util::AbstractQueue<::jxx::lang::String>, StringDeque>);
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::AbstractCollection<::jxx::lang::String>, StringDeque>));
}

TEST(ArrayDequeInheritanceParity, ImplementsSourceInterfaces) {
    static_assert(std::is_base_of_v<
        ::jxx::util::Deque<::jxx::lang::String>, StringDeque>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, StringDeque>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, StringDeque>);
    SUCCEED();
}

TEST(ArrayDequeInheritanceParity, ClassMetadataIsAvailableForConcreteSpecialization) {
    EXPECT_NE(nullptr, StringDeque::Class());
}
