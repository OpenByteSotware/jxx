#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayDeque.h"
#include "util/jxx.util.ClassInitialization.h"

namespace
{
    using ObjectDeque = ::jxx::util::ArrayDeque<::jxx::lang::Object>;

    TEST(ArrayDequeClassInitialization, RegistersObjectSpecialization)
    {
        ::jxx::util::initializeClasses();
        const auto type = ObjectDeque::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(ArrayDequeSerializationInitialization, RegistersSourceUid)
    {
        ::jxx::util::initializeClasses();
        const auto metadata =
            ::jxx::io::serialization::SerializationClassRegistry::resolve(
                ObjectDeque::Class()->getName()->utf8());
        EXPECT_EQ(metadata.serialVersionUID,
            static_cast<::jxx::lang::jlong>(2340985798034038923LL));
        EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    }

    TEST(ArrayDequeSerialization, RoundTripsElementsInDequeOrder)
    {
        ::jxx::util::initializeClasses();
        auto source = ::jxx::NEW<ObjectDeque>();
        source->addLast(::jxx::NEW<::jxx::lang::String>("first"));
        source->addLast(::jxx::NEW<::jxx::lang::String>("second"));

        auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
        {
            auto out = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
            out->writeObject(source);
            out->flush();
        }

        auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
            bytes->toByteArray());
        auto in = ::jxx::NEW<::jxx::io::ObjectInputStream>(input);
        auto restored = ::jxx::CAST<ObjectDeque>(in->readObject());

        ASSERT_NE(restored, nullptr);
        ASSERT_EQ(restored->size(), 2);
        auto first = ::jxx::CAST<::jxx::lang::String>(restored->removeFirst());
        auto second = ::jxx::CAST<::jxx::lang::String>(restored->removeFirst());
        ASSERT_NE(first, nullptr);
        ASSERT_NE(second, nullptr);
        EXPECT_EQ(first->utf8(), "first");
        EXPECT_EQ(second->utf8(), "second");
    }
}
