#include <gtest/gtest.h>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Arrays.h"

TEST(AbstractSecurityControllerParity, CopiesAndComparesSecurityMasks) {
    auto source = ::jxx::NEW<::jxx::lang::ByteArrayType>(67);
    for (::jxx::lang::jint index = 0; index < source->length; ++index) {
        (*source)[index] = static_cast<::jxx::lang::jbyte>(index);
    }

    auto front = ::jxx::util::Arrays::copyOfRange(source, 2, 34);
    auto rear = ::jxx::util::Arrays::copyOfRange(source, 34, 66);
    ASSERT_EQ(32, front->length);
    ASSERT_EQ(32, rear->length);
    EXPECT_EQ(2, static_cast<int>((*front)[0]));
    EXPECT_EQ(34, static_cast<int>((*rear)[0]));
    EXPECT_TRUE(::jxx::util::Arrays::equals(front, front));
    EXPECT_FALSE(::jxx::util::Arrays::equals(front, rear));
}

TEST(AbstractSecurityControllerParity, ArrayListStoresByteArrayReferences) {
    using ByteArrayType = ::jxx::lang::ByteArrayType;
    auto records = ::jxx::NEW<::jxx::util::ArrayList<ByteArrayType>>(1);
    auto credential = ::jxx::NEW<ByteArrayType>(16);
    (*credential)[0] = 42;

    EXPECT_TRUE(records->add(credential));
    EXPECT_FALSE(records->isEmpty());
    EXPECT_EQ(1, records->size());
    EXPECT_EQ(credential.get(), records->get(0).get());
    EXPECT_EQ(credential.get(), records->remove(0).get());
    EXPECT_TRUE(records->isEmpty());
}

TEST(AbstractSecurityControllerParity, FillAndArraycopyPreserveMaskBehavior) {
    auto source = ::jxx::NEW<::jxx::lang::ByteArrayType>(32);
    auto destination = ::jxx::NEW<::jxx::lang::ByteArrayType>(32);
    ::jxx::util::Arrays::fill(source,
        static_cast<::jxx::lang::jbyte>(0x7F));
    ::jxx::lang::System::arraycopy(source, 0, destination, 0, 32);
    EXPECT_TRUE(::jxx::util::Arrays::equals(source, destination));
    ::jxx::util::Arrays::fill(destination,
        static_cast<::jxx::lang::jbyte>(0));
    EXPECT_EQ(0, static_cast<int>((*destination)[31]));
}

TEST(AbstractSecurityControllerParity, SignedFloorMapsIntoMaskIndex) {
    auto mask = ::jxx::NEW<::jxx::lang::ByteArrayType>(32);
    const ::jxx::lang::jint floor = -1;
    const ::jxx::lang::jint normalized = floor + 256;
    const ::jxx::lang::jint byteIndex = normalized / 8;
    const ::jxx::lang::jint bitIndex = normalized % 8;
    (*mask)[byteIndex] = static_cast<::jxx::lang::jbyte>(0x80);
    const ::jxx::lang::jint bitmask[] = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    EXPECT_NE(0,
        static_cast<::jxx::lang::jint>((*mask)[byteIndex]) & bitmask[bitIndex]);
}
