#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReferenceArray.h"

using ErasedAtomicReferenceArray =
    ::jxx::util::concurrent::atomic::AtomicReferenceArray<::jxx::lang::Object>;

TEST(AtomicReferenceArraySerialization, RoundTripsValuesAndNulls) {
    ::jxx::initialize();
    const auto source = ::jxx::NEW<ErasedAtomicReferenceArray>(3);
    source->set(0, ::jxx::NEW<::jxx::lang::String>("first"));
    source->set(2, ::jxx::NEW<::jxx::lang::String>("third"));
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    const auto restored = std::dynamic_pointer_cast<ErasedAtomicReferenceArray>(input->readObject());
    ASSERT_NE(restored, nullptr); ASSERT_EQ(restored->length(), 3);
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->get(0))->utf8(), "first");
    EXPECT_EQ(restored->get(1), nullptr);
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->get(2))->utf8(), "third");
}
