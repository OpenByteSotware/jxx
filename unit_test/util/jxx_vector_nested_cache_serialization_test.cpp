#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/jxx.util.SerializationRegistration.h"
#include "util/jxx.util.Vector.h"

namespace {

using Element = ::jxx::lang::String;
using InnerVector = ::jxx::util::Vector<Element>;
using OuterVector = ::jxx::util::Vector<InnerVector>;
using CacheMap = ::jxx::util::concurrent::ConcurrentHashMap<
    ::jxx::lang::Integer, OuterVector>;

::jxx::Ptr<::jxx::lang::Object> roundTrip(
    const ::jxx::Ptr<::jxx::lang::Object>& source)
{
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));
    output->writeObject(source);
    output->close();
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::CAST<::jxx::io::InputStream>(inputBytes));
    return input->readObject();
}

TEST(VectorSerializationParity, PreservesCapacityIncrementOrderNullsAndHandles)
{
    ::jxx::util::serialization::registerVector<Element>();
    auto shared = ::jxx::NEW<Element>("shared");
    auto source = ::jxx::NEW<InnerVector>(8, 3);
    source->add(shared);
    source->add(nullptr);
    source->add(shared);

    auto restored = ::jxx::CAST<InnerVector>(roundTrip(
        ::jxx::CAST<::jxx::lang::Object>(source)));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->size(), 3);
    EXPECT_EQ(restored->capacity(), 8);
    EXPECT_EQ(restored->get(1), nullptr);
    EXPECT_EQ(restored->get(0), restored->get(2));
    EXPECT_EQ(restored->get(0)->utf8(), "shared");
}

} // namespace
