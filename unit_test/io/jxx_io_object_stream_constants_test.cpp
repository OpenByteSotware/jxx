#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.ObjectStreamConstants.h"

TEST(ObjectStreamConstantsParity, StreamClassesImplementConstants) {
    EXPECT_TRUE((std::is_base_of_v<jxx::io::ObjectStreamConstants, jxx::io::ObjectInputStream>));
    EXPECT_TRUE((std::is_base_of_v<jxx::io::ObjectStreamConstants, jxx::io::ObjectOutputStream>));
}
TEST(ObjectStreamConstantsParity, ConstantsMatchSourceContract) {
    using C = jxx::io::ObjectStreamConstants;
    EXPECT_EQ(static_cast<unsigned short>(C::STREAM_MAGIC), 0xACEDu);
    EXPECT_EQ(C::STREAM_VERSION, 5);
    EXPECT_EQ(C::baseWireHandle, 0x7E0000);
    EXPECT_EQ(static_cast<unsigned char>(C::TC_NULL), 0x70u);
    EXPECT_EQ(static_cast<unsigned char>(C::TC_ENUM), 0x7Eu);
    EXPECT_EQ(C::PROTOCOL_VERSION_1, 1);
    EXPECT_EQ(C::PROTOCOL_VERSION_2, 2);
}
