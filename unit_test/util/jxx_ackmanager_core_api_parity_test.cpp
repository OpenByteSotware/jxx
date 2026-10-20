#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Math.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Vector.h"

TEST(AckManagerCoreParity, AddressSplitHashAndPacketIdentifierRange)
{
    ::jxx::initialize();
    auto address = ::jxx::NEW<::jxx::lang::String>("127.0.0.1");
    auto octets = address->split(
        ::jxx::NEW<::jxx::lang::String>("\\."));
    ASSERT_NE(octets, nullptr);
    ASSERT_EQ(octets->length, 4);
    EXPECT_EQ((*octets)[3]->utf8(), "1");
    EXPECT_EQ(::jxx::lang::Integer::parseInt((*octets)[3]), 1);

    const auto absoluteHash = ::jxx::lang::Math::abs(address->hashCode());
    const auto packetIdentifier = (absoluteHash % 5000) + 1000;
    EXPECT_GE(packetIdentifier, 1000);
    EXPECT_LE(packetIdentifier, 5999);
    EXPECT_NE(::jxx::net::InetAddress::getByName(address), nullptr);
}

TEST(AckManagerCoreParity, VectorWindowAndArrayAckOperations)
{
    ::jxx::initialize();
    using Object = ::jxx::lang::Object;
    auto recent = ::jxx::NEW<::jxx::util::Vector<Object>>();
    recent->setSize(5);
    recent->trimToSize();
    ASSERT_EQ(recent->size(), 5);
    EXPECT_EQ(recent->capacity(), 5);
    recent->removeElementAt(0);
    EXPECT_EQ(recent->size(), 4);

    auto packet = ::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    for (::jxx::lang::jint index = 0; index < packet->length; ++index)
        (*packet)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    auto ack = ::jxx::util::Arrays::copyOfRange(packet, 0, 4);
    ASSERT_EQ(ack->length, 4);
    EXPECT_TRUE(::jxx::util::Arrays::equals(
        ack, ::jxx::util::Arrays::copyOfRange(packet, 0, 4)));
    EXPECT_FALSE(::jxx::util::Arrays::equals(
        ack, ::jxx::util::Arrays::copyOfRange(packet, 1, 5)));
}
