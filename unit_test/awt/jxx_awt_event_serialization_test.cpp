#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.AWTEvent.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(JxxAwtEventSerializationTest, RegistersJava8Descriptor)
{
    ::jxx::initialize();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::awt::AWTEvent::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(-1825314779160409405LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(JxxAwtEventSerializationTest, RoundTripsSourceAndEventState)
{
    ::jxx::initialize();
    const auto source = ::jxx::NEW<::jxx::lang::String>("source");
    const auto event = ::jxx::NEW<::jxx::awt::AWTEvent>(
        ::jxx::CAST<::jxx::lang::Object>(source), 1234);

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(event));
    output->flush();

    const auto inputBytes =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    const auto restored = ::jxx::CAST<::jxx::awt::AWTEvent>(
        input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ(1234, restored->getID());
    EXPECT_FALSE(restored->isConsumed());
    const auto restoredSource =
        ::jxx::CAST<::jxx::lang::String>(restored->getSource());
    ASSERT_NE(nullptr, restoredSource);
    EXPECT_EQ("source", restoredSource->utf8());
}

} // namespace
