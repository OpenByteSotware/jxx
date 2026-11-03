#include <type_traits>
#include <gtest/gtest.h>
#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/jxx.awt.ComponentOrientation.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"

TEST(AwtComponentOrientationParity, ImplementsSerializable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::awt::ComponentOrientation>));
}

TEST(AwtComponentOrientationParity, SerializationRoundTrip) {
    ::jxx::awt::initializeClasses();
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto out = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    out->writeObject(::jxx::awt::ComponentOrientation::RIGHT_TO_LEFT);
    out->flush();
    const auto source = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    const auto in = ::jxx::NEW<::jxx::io::ObjectInputStream>(source);
    const auto restored = ::jxx::CAST<::jxx::awt::ComponentOrientation>(in->readObject());
    ASSERT_NE(nullptr, restored);
    EXPECT_TRUE(restored->isHorizontal());
    EXPECT_FALSE(restored->isLeftToRight());
}
