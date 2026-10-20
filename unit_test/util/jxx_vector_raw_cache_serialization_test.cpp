#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/jxx.util.SerializationRegistration.h"
#include "util/jxx.util.Vector.h"

namespace {

using Object = ::jxx::lang::Object;
using RawVector = ::jxx::util::Vector<Object>;
using CacheMap = ::jxx::util::concurrent::ConcurrentHashMap<
    ::jxx::lang::Integer, RawVector>;

::jxx::Ptr<Object> roundTrip(const ::jxx::Ptr<Object>& source)
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

TEST(VectorSerializationParity, RoundTripsRawNestedVectorCacheMap)
{
    ::jxx::initialize();
    ::jxx::util::serialization::registerRawVectorCacheGraph();

    auto inner = ::jxx::NEW<RawVector>();
    inner->add(::jxx::CAST<Object>(
        ::jxx::NEW<::jxx::lang::String>("packet")));
    auto outer = ::jxx::NEW<RawVector>();
    outer->add(::jxx::CAST<Object>(inner));
    auto source = ::jxx::NEW<CacheMap>();
    source->put(::jxx::lang::Integer::valueOf(0), outer);

    auto restored = ::jxx::CAST<CacheMap>(roundTrip(
        ::jxx::CAST<Object>(source)));
    ASSERT_NE(restored, nullptr);
    auto restoredOuter = restored->get(::jxx::lang::Integer::valueOf(0));
    ASSERT_NE(restoredOuter, nullptr);
    ASSERT_EQ(restoredOuter->size(), 1);
    auto restoredInner = ::jxx::CAST<RawVector>(restoredOuter->get(0));
    ASSERT_NE(restoredInner, nullptr);
    ASSERT_EQ(restoredInner->size(), 1);
    auto restoredString = ::jxx::CAST<::jxx::lang::String>(
        restoredInner->get(0));
    ASSERT_NE(restoredString, nullptr);
    EXPECT_EQ(restoredString->utf8(), "packet");
}

TEST(VectorSerializationParity, InitializationAndRegistrationAreIdempotent)
{
    ::jxx::initialize();
    ::jxx::initialize();
    EXPECT_NO_THROW(
        ::jxx::util::serialization::registerRawVectorCacheGraph());
    EXPECT_NO_THROW(
        ::jxx::util::serialization::registerRawVectorCacheGraph());
}

} // namespace
