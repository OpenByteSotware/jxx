#include <gtest/gtest.h>

#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/jxx.awt.Polygon.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"

namespace
{
    TEST(PolygonClassInitialization, RegistersClassMetadata)
    {
        ::jxx::awt::initializeClasses();
        const auto type = ::jxx::awt::Polygon::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_EQ(type->getName()->utf8(), "jxx.awt.Polygon");
    }

    TEST(PolygonSerializationInitialization, RegistersSourceSerialVersionUid)
    {
        ::jxx::awt::initializeClasses();
        const auto metadata =
            ::jxx::io::serialization::SerializationClassRegistry::resolve(
                ::jxx::awt::Polygon::Class()->getName()->utf8());
        EXPECT_EQ(metadata.serialVersionUID,
            static_cast<::jxx::lang::jlong>(-6460061437900069969LL));
        EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    }

    TEST(PolygonSerialization, RoundTripsCoordinates)
    {
        ::jxx::awt::initializeClasses();
        auto source = ::jxx::NEW<::jxx::awt::Polygon>();
        source->addPoint(3, 5);
        source->addPoint(-7, 11);
        source->addPoint(19, -23);

        auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
        {
            auto out = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
            out->writeObject(source);
            out->flush();
        }

        auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
            bytes->toByteArray());
        auto in = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
        auto restored = ::jxx::CAST<::jxx::awt::Polygon>(in->readObject());

        ASSERT_NE(restored, nullptr);
        ASSERT_EQ(restored->npoints, 3);
        EXPECT_EQ((*restored->xpoints)[0], 3);
        EXPECT_EQ((*restored->ypoints)[0], 5);
        EXPECT_EQ((*restored->xpoints)[1], -7);
        EXPECT_EQ((*restored->ypoints)[1], 11);
        EXPECT_EQ((*restored->xpoints)[2], 19);
        EXPECT_EQ((*restored->ypoints)[2], -23);
    }
}
